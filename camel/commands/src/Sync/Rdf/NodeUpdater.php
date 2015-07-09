<?php

/**
 * This file is part of Islandora.
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 *
 * PHP Version 5.5.9
 *
 * @category Islandora
 * @package  Islandora\Sync\Rdf
 * @author   Daniel Lamb <daniel@discoverygarden.ca>
 * @license  http://www.gnu.org/licenses/gpl-3.0.en.html GPL
 * @link     http://www.islandora.ca
 */

use Islandora\PrefixEscapingCommand;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;

/**
 * Updates a Drupal node from Fedora RDF.
 *
 * @category Islandora
 * @package  Islandora\Sync\Rdf
 * @author   Daniel Lamb <daniel@discoverygarden.ca>
 * @license  http://www.gnu.org/licenses/gpl-3.0.en.html GPL
 * @link     http://www.islandora.ca
 */
class NodeUpdater extends PrefixEscapingCommand
{
    /**
     * Sets command configuration.
     *
     * @return null
     */
    protected function configure()
    {
        $this->setName('rdf:updateNode')
            ->setDescription(
                "Updates a Drupal node from Fedora RDF."
            );
    }

    /**
     * Updates a Drupal node from Fedora RDF.
     *
     * @param InputInterface  $input  An InputInterface instance
     * @param OutputInterface $output An OutputInterface instance
     *
     * @return null
     */
    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $node = &$this->data['node'];
        $rdf = &$this->data['rdf'][0];

        $this->updateNodeFromRdf($node, $rdf);

        $output->write(json_encode($node));
    }

    /**
     * Utility function for updating a Drupal node from RDF.
     *
     * @param array $node A Drupal node
     * @param array $rdf  Some ld+json RDF
     *
     * @return null
     */
    protected function updateNodeFromRdf(&$node, $rdf)
    {
        $rdf_mapping = $node['rdf_mapping'];
        $namespaces = $node['rdf_namespaces'];
        $language = $node['language'];

        // Iterate over the mapping to update the ndoe
        foreach ($rdf_mapping as $key => $value) {
            // We ignore some fields.
            if (in_array($key, $this->generateIgnoreList())) {
                 continue;
            }

            // Grab the predicates for the mapping and
            $predicates = $value['predicates'];
            foreach ($predicates as $predicate) {
                // Escape the predicate since the ld+json from Fedora has namespaces
                // declared in full.
                $escaped_predicate = $this->escapePrefix($predicate, $namespaces);

                // Carry on if there is no value for the predicate in the RDF.
                if (!isset($rdf[$escaped_predicate])) {
                    continue;
                }

                // Handle entity properties.
                // Not sure how to deal with multiple values, so...
                // Last one wins?
                if ($this->isEntityProperty($key)) {
                    foreach ($rdf[$escaped_predicate] as $rdf_entry) {
                        $node[$key] = $rdf_entry['@value'];
                    }
                } else {
                    // Otherwise it's a field, with it's own array structure based
                    // on language.
                    $field = array();
                    $field[$language] = array();

                    foreach ($rdf[$escaped_predicate] as $rdf_entry) {
                        // Special case: field_fedora_has_parent uses @id instead
                        // of @value.
                        if (strcmp("field_fedora_has_parent", $key) == 0) {
                            $field[$language][] = array(
                                'value' => $rdf_entry['@id']
                            );
                        } else {
                            $field[$language][] = array(
                                'value' => $rdf_entry['@value']
                            );
                        }
                    }

                    $node[$key] = $field;
                }
            }
        }

        // Another special case:
        // Fedora path is the @id entry of the rdf, and is NOT in the rdf_mapping
        // from Fedora.
        $node['field_fedora_path'] = array(
            $language => array(
                array(
                    'value' => $rdf['@id'],
                ),
            ),
        );
    }

    /**
     * Generates a list of properties to ignore for this command.
     *
     * @return array
     */
    protected function generateIgnoreList()
    {
        return array(
            "rdftype",
            "uuid",
            "body",
            "lastActivity",
        );
    }

    /**
     * Checks to see if a property is an entity property (e.g. not a field).
     *
     * @param String $property_name Name of the property on the node
     *
     * @return null
     */
    protected function isEntityProperty($property_name)
    {
        $entity_properties = array(
            "vid",
            "uid",
            "title",
            "log",
            "status",
            "comment",
            "promote",
            "sticky",
            "vuuid",
            "nid",
            "type",
            "language",
            "created",
            "changed",
            "tnid",
            "translate",
            "uuid",
            "revision_timestamp",
            "revision_uid",
            "cid",
            "last_comment_timestamp",
            "last_comment_name",
            "last_commment_uid",
            "comment_count",
            "name",
            "picture",
            "data"
        );

        return in_array($property_name, $entity_properties);
    }
}
