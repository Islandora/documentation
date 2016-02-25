<?php

namespace Islandora\CollectionService\Controller;

use Silex\Application;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;
use Islandora\Chullo\Uuid\IUuidGenerator;

class CollectionController {

    protected $uuidGenerator;

    public function __construct(IUuidGenerator $uuidGenerator) {
        $this->uuidGenerator = $uuidGenerator;
    }

    public function create(Application $app, Request $request, $id) {
        $tx = $request->query->get('tx', "");

        //Check for format
        $format = NULL;
        try {
          $format = \EasyRdf_Format::getFormat($contentType = $request->headers->get('Content-Type', 'text/turtle'));
        } catch (\EasyRdf_Exception $e) {
          $app->abort(415, $e->getMessage());
        }

        //Now check if body can be parsed in that format
        if ($format) { //EasyRdf_Format
          //@see http://www.w3.org/2011/rdfa-context/rdfa-1.1 for defaults
          \EasyRdf_Namespace::set('pcdm', 'http://pcdm.org/models#');
          \EasyRdf_Namespace::set('nfo', 'http://www.semanticdesktop.org/ontologies/2007/03/22/nfo/v1.1/');
          \EasyRdf_Namespace::set('isl', 'http://www.islandora.ca/ontologies/2016/02/28/isl/v1.0/');
          \EasyRdf_Namespace::set('ldp', 'http://www.w3.org/ns/ldp');

          //Fake IRI, default LDP one for current resource "<>" is not a valid IRI!
          $fakeUuid = $this->uuidGenerator->generateV5("derp");
          $fakeIri = new \EasyRdf_ParsedUri('urn:uuid:' . $fakeUuid);

          $graph = new \EasyRdf_Graph();
          try {
            $graph->parse($request->getContent(), $format->getName(), $fakeIri);
          } catch (\EasyRdf_Exception $e) {
            $app->abort(415, $e->getMessage());
          }
          //Add a pcmd:Collection type
          $graph->resource($fakeIri, 'pcdm:Collection');

          //Check if we got an UUID inside posted RDF. We won't validate it here because it's the caller responsability
          if (NULL != $graph->countValues($fakeIri, '<http://www.semanticdesktop.org/ontologies/2007/03/22/nfo/v1.1/uuid>')) {
            $existingUuid = $graph->getLiteral($fakeIri, '<http://www.semanticdesktop.org/ontologies/2007/03/22/nfo/v1.1/uuid>');
            $graph->addResource($fakeIri, 'http://www.islandora.ca/ontologies/2016/02/28/isl/v1.0/hasURN', 'urn:uuid:'.$existingUuid); //Testing an Islandora Ontology!
          } else {
            //No UUID from the caller in RDF, lets put something there
            $graph->addLiteral($fakeIri,"http://www.semanticdesktop.org/ontologies/2007/03/22/nfo/v1.1/uuid",$fakeUuid); //Keeps compat for now with other services
            $graph->addResource($fakeIri,"http://www.islandora.ca/ontologies/2016/02/28/isl/v1.0/hasURN",'urn:uuid:'.$fakeUuid); //Testing an Islandora Ontology
          }
          //Restore LDP <> IRI on serialised graph
          $pcmd_collection_rdf= str_replace($fakeIri, '', $graph->serialise('turtle'));
        }

        $urlRoute = $request->getUriForPath('/islandora/resource/');

        $subRequestPost = Request::create($urlRoute.$id, 'POST', array(), $request->cookies->all(), array(), $request->server->all(), $pcmd_collection_rdf);
        $subRequestPost->query->set('tx', $tx);
        $subRequestPost->headers->set('Content-Type', 'text/turtle');
        $responsePost = $app->handle($subRequestPost, HttpKernelInterface::SUB_REQUEST, false);

        if (201 == $responsePost->getStatusCode()) {// OK, collection created
          //Lets take the location header in the response
          $indirect_container_rdf = $app['twig']->render('createIndirectContainerfromTS.ttl', array(
            'resource' => $responsePost->headers->get('location'),
          ));

          $subRequestPut = Request::create($urlRoute.$id, 'PUT', array(), $request->cookies->all(), array(), $request->server->all(), $indirect_container_rdf);
          $subRequestPut->query->set('tx', $tx);
          $subRequestPut->headers->set('Slug', 'members');
          //Can't use in middleware, but needed. Without Fedora 4 throws big java errors!
          $subRequestPut->headers->set('Host', $app['config']['islandora']['fedoraHost'], TRUE);
          $subRequestPut->headers->set('Content-Type', 'text/turtle');
          //Here is the thing. We don't know if UUID of the collection we just created is already in the tripple store.
          //So what to do? We could just try to use our routes directly, but UUID check agains triplestore we could fail!
          //lets invoke the controller method directly
          $responsePut = $app['islandora.resourcecontroller']->put($app, $subRequestPut, $responsePost->headers->get('location'), "members");
          if (201 == $responsePut->getStatusCode()) {// OK, indirect container created
            //Include headers from the parent one, some of the last one. Basically rewrite everything
            $putHeaders = $responsePut->getHeaders();
            //Guzzle psr7 response objects are inmutable. So we have to make this an array and add directly
            $putHeaders['Link'] = array('<'.$responsePut->getBody().'>; rel="alternate"');
            $putHeaders['Link'] = array('<'.$urlRoute.$fakeUuid.'/members>; rel="hub"');
            $putHeaders['Location'] = array($urlRoute.$fakeUuid);
            //Should i care about the etag?
            return new Response($putHeaders['Location'][0], 201, $putHeaders);
          }

          return $responsePut;
        }
        //Abort if PCDM collection object could not be created
        $app->abort($responsePost->getStatusCode(), 'Failed creating PCDM Collection');
    }
}
