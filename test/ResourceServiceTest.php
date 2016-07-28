<?php
/**
 * Created by PhpStorm.
 * User: whikloj
 * Date: 2016-07-25
 * Time: 11:18 AM
 */

namespace Islandora\CLAW;


use ML\JsonLD\JsonLD;
use Symfony\Component\HttpKernel\HttpKernelInterface;
use Islandora\CLAW\StackTests;
use GuzzleHttp\Psr7\Response;

class ResourceService extends StackTests
{

  /**
   * Get the Fedora Root resource
   *
   * @group IntegrationTests
   */
  public function testGetRoot()
  {
    $response = $this->client->claw->request('GET', "/islandora/resource");
    $this->assertEquals($response->getStatusCode(), 200, "Did not get the Root resource");
  }

  /**
   * Create a RdfSource container with POST
   *
   * @group IntegrationTests
   */
  public function testCreateContainerPOST()
  {
    $uuid = $this->uuid_gen->generateV4();
    $rdf = $this->twig->render('add_uuid.ttl', array('uuid' => $uuid));
    $response = $this->client->claw->request('POST', "/islandora/resource", [
      'body' => $rdf,
      'headers' => [
        'Content-type' => 'text/turtle'
      ]
    ]);
    $this->assertEquals($response->getStatusCode(), 201, "Did not create container");

    if ($response->hasHeader('Location')) {
      $location = $response->getHeader('Location');
      if (is_array($location)) {
        $location = reset($location);
      }

      $fedora_res = $this->client->fedora->request('GET', $location);
      $this->assertEquals($fedora_res->getStatusCode(), 200, "Fedora does not have the location");
    }

    // Wait for data to index.
    sleep(1);

    $responseGet = $this->client->claw->request('GET', "/islandora/resource/$uuid", [
      'headers' => [
        'Accept' => 'application/ld+json'
      ]
    ]);
    $this->assertEquals($responseGet->getStatusCode(), 200, 'Could not get back UUID ' . $uuid);

  }

  /**
   * Create a RdfSource container with POST
   *
   * @group IntegrationTests
   */
  public function testCreateContainerPUT()
  {
    $uuid = $this->uuid_gen->generateV4();
    $rdf = $this->twig->render('add_uuid.ttl', array('uuid' => $uuid));

    // Delete container if it exists
    $this->deleteFedoraResource('object1');

    $response = $this->client->claw->request('PUT', "/islandora/resource//object1", [
      'body' => $rdf,
      'headers' => [
        'Content-type' => 'text/turtle',
        'Content-length' => strlen($rdf)
      ]
    ]);
    $this->assertEquals($response->getStatusCode(), 201, "Did not create container");

    if ($response->hasHeader('Location')) {
      $location = $response->getHeader('Location');
      if (is_array($location)) {
        $location = reset($location);
      }

      $fedora_res = $this->client->fedora->request('GET', $location);
      $this->assertEquals($fedora_res->getStatusCode(), 200, "Fedora does not have the location");
    }

    // Wait for data to index.
    sleep(1);

    $responseGet = $this->client->claw->request('GET', "/islandora/resource/$uuid", [
      'headers' => [
        'Accept' => 'application/ld+json'
      ]
    ]);
    $this->assertEquals($responseGet->getStatusCode(), 200, 'Could not get back UUID ' . $uuid);

  }

  /**
   * Test create a container inside a container
   *
   * @group IntegrationTests
   */
  public function testCreateNestedContainers()
  {
    $uuid1 = $this->uuid_gen->generateV4();
    $uuid2 = $this->uuid_gen->generateV4();
    $rdf1 = $this->twig->render('add_uuid.ttl', array('uuid' => $uuid1));
    $rdf2 = $this->twig->render('add_uuid.ttl', array('uuid' => $uuid2));

    $response1 = $this->client->claw->request('POST', "/islandora/resource", [
      'body' => $rdf1,
      'headers' => [
        'Content-type' => 'text/turtle',
        'Content-length' => strlen($rdf1)
      ]
    ]);

    $this->assertEquals($response1->getStatusCode(), 201, "Did not create Rdf Source container.");

    sleep(1);

    $response2 = $this->client->claw->request('POST', "/islandora/resource/$uuid1", [
      'body' => $rdf2,
      'headers' => [
        'Content-type' => 'text/turtle',
        'Content-length' => strlen($rdf2)
      ]
    ]);

    $this->assertEquals($response2->getStatusCode(), 201, "Did not create nested Rdf Source container.");

    sleep(1);

    $get_resp1 = $this->client->claw->request('GET', "/islandora/resource/$uuid1", [
      'Accept' => 'application/ld+json'
    ]);
    $this->assertEquals($get_resp1->getStatusCode(), 200, "Did not get resource ($uuid1)");

    $get_resp2 = $this->client->claw->request('GET', "/islandora/resource/$uuid2", [
      'Accept' => 'application/ld+json'
    ]);
    $this->assertEquals($get_resp2->getStatusCode(), 200, "Did not get resource ($uuid1)");

    // TODO: check that the ldp:contains of get_resp1 = resource at get_resp2

  }

  /**
   * Test uploading a binary
   *
   * @group IntegrationTests
   */
  public function testPostBinary()
  {
    $uuid = $this->uuid_gen->generateV4();
    $rdf = file_get_contents(__DIR__ . '/../resources/images/general_montgomery/montgomery.json');
    $content = file_get_contents(__DIR__ . '/../resources/images/general_montgomery/montgomery.jpg');
    $sha1 = sha1($content);
    $response = $this->client->claw->request('POST', "/islandora/resource", [
      'body' => $content,
      'headers' => [
        'Content-type' => 'image/jpeg',
        'Content-length' => strlen($content)
      ]
    ]);

    $this->assertEquals($response->getStatusCode(), 201, "Non Rdf Source not created");

    $this->assertTrue($response->hasHeader('Location'), "No Location header");

    $location = $response->getHeader('Location');
    if (is_array($location)) {
      $location = reset($location);
    }
    $patch = $this->twig->render('add_uuid.sparql', array('uuid' => $uuid));

    $p_resp = $this->client->fedora->request('PATCH', $location . '/fcr:metadata', [
      'body' => $patch,
      'headers' => [
        'Content-type' => 'application/sparql-update',
        'Content-length' => strlen($patch)
      ]
    ]);
    $this->assertEquals($p_resp->getStatusCode(), 204, "Unable to add UUID to binary");

    sleep(1);

    $resp = $this->client->claw->request('GET', "/islandora/resource/$uuid");
    $this->assertEquals($resp->getStatusCode(), 200, "UUID $uuid not found");

    $fix_resp = $this->client->claw->request('GET', "/islandora/resource/$uuid/fcr:fixity", [
      'headers' => [
        'Accept' => 'application/ld+json'
      ]
    ]);
    // TODO: Parse JSON-LD and compare sha1 to $sha1
    $document = JsonLD::getDocument($fix_resp->getBody());


  }

  /**
   * @group dev
   */
  public function testParseJsonLD()
  {
    $resp = $this->client->fedora->request(
      'GET',
      "http://localhost:8080/fcrepo/rest/63/d0/88/fb/63d088fb-ceae-408a-b727-3d7171488be3/fcr:fixity",
      [
        'headers' => [
          'Accept' => 'application/ld+json'
        ]
      ]);
    $this->assertEquals($resp->getStatusCode(), 200, "Couldn't get fixity result");

    print "body is " . $resp->getBody();

    $document = JsonLD::getDocument((string) $resp->getBody());

    print "graphs\n";
    foreach ($document->getGraphNames() as $graph) {
      error_log($graph);
    }
    print "end graphs\nnodes\n";
    foreach ($document->getGraph()->getNodes() as $node) {
      print "graphname is " . $node->
      print "id is " . $node->getId() . "\n";
    }
    print "end nodes\n";
    print JsonLD::toString($document);
  }
}