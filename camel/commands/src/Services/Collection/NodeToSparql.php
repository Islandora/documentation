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
 * @package  Islandora\Services\Collection
 * @author   Daniel Lamb <daniel@discoverygarden.ca>
 * @license  http://www.gnu.org/licenses/gpl-3.0.en.html GPL
 * @link     http://www.islandora.ca
 */

use Islandora\PrefixEscapingCommand;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;

/**
 * Converts Drupal node JSON for a collection to a SPARQL Update query.
 *
 * @category Islandora
 * @package  Islandora\Services\Collection
 * @author   Daniel Lamb <daniel@discoverygarden.ca>
 * @license  http://www.gnu.org/licenses/gpl-3.0.en.html GPL
 * @link     http://www.islandora.ca
 */
class NodeToSparql extends PrefixEscapingCommand
{
    /**
     * Sets command configuration.
     *
     * @return null
     */
    protected function configure()
    {
        $this->setName('collectionService:nodeToSparql')
            ->setDescription(
                "Converts Drupal node JSON for a collection to a SPARQL Update query"
            );
    }

    /**
     * Converts Drupal node JSON for a collection to a SPARQL Update query.
     *
     * @param InputInterface  $input  An InputInterface instance
     * @param OutputInterface $output An OutputInterface instance
     *
     * @return null
     */
    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $node = $this->data;

        $rdf_mapping = $node['rdf_mapping'];
        $namespaces = $node['rdf_namespaces'];
        $counter = 0;

        $triples_to_remove = array();
        $triples_to_add = array();

        // Ignore Fedora system properties that are immutable.
        $ignore_list = array(
            "field_fedora_has_parent",
            "field_fedora_path",
        );

        foreach ($rdf_mapping as $key => $value) {
            // Set rdf:type.
            if (strcmp("rdftype", $key) == 0) {
                $triples_to_remove[] = "<> " .
                    '<' . $this->escapePrefix("rdf:type", $namespaces) . '>' .
                    " ?o{$counter} .";

                $counter++;

                foreach ($value as $type) {
                    $triples_to_add[] = "<> " .
                        '<' . $this->escapePrefix("rdf:type", $namespaces) . "> " .
                        '<' . $this->escapePrefix($type, $namespaces) . "> .";
                }
            } else if (in_array($key, $ignore_list)) {
                // Ignore Fedora system properties that are immutable.
                continue;
            } else {
                // Convert all other entity properties and fields to RDF.
                foreach ($value['predicates'] as $predicate) {
                    $escaped_predicate = $this->escapePrefix(
                        $predicate, $namespaces
                    );

                    // Continue on if this node doesn't have the field
                    // (e.g. bodyless node).
                    if (!isset($node[$key]) || empty($node[$key])) {
                        continue;
                    }

                    // Fields come through as arrays.
                    if (is_array($node[$key])) {
                        $language = $node['language'];
                        foreach ($node[$key][$language] as $field) {
                            $triples_to_remove[]
                                = "<> <$escaped_predicate> ?o$counter .";
                            $counter++;
                            $object = $this->_formatObject($field['value'], $value);
                            $triples_to_add[] = "<> <$escaped_predicate> $object .";
                        }
                    } else {
                        // Entity properties come through as strings.
                        $triples_to_remove[]
                            = "<> <$escaped_predicate> ?o$counter .";
                        $counter++;
                        $object = $this->_formatObject($node[$key], $value);
                        $triples_to_add[] = "<> <$escaped_predicate> $object .";
                    }
                }
            }
        }

        $output->write(
            $this->_buildQuery($namespaces, $triples_to_remove, $triples_to_add)
        );
    }

    /**
     * Formats the object in a triple based on user input from Drupal.
     *
     * @param string $object  The object that needs formatting
     * @param array  $rdf_map RDF mapping entry from Drupal UI
     *
     * @return string The properly formatted object
     */
    private function _formatObject($object, $rdf_map)
    {
        // Do nothing for the 'rel' types.  This will have to get reworked since
        // uid gets mapped to sioc:has_creator by default, but the value is
        // the uid itself, not http://your_drupal/users/the_uid....
        if (isset($rdf_map['type']) && strcmp($rdf_map['type'], "rel") == 0) {
            return $object;
        }

        if (isset($rdf_map['datatype'])) {
            $datatype = $rdf_map['datatype'];

            // Add angle brackets if it's a URI.
            if (strcmp($datatype, "xsd:anyURI") == 0) {
                return "<" . $object . ">";
            } elseif (strcmp($datatype, "xsd:string") == 0) {
                // Add quotes if it's a string.
                return '"' . $object . '"';
            } elseif (strcmp($datatype, "xsd:dateTime") == 0) {
                // Do proper dateTime formatting if it's a date.
                // Note:  We can't actually use the callback provided in the RDF
                // mapping since we're not in Drupal land.  Instead, we're just
                // doing what that callback does anyway.
                return '"' .
                    date('c', intval($object)) .
                    '"^^<http://www.w3.org/2001/XMLSchema#dateTime>';
            } elseif (strcmp($datatype, "xsd:integer") == 0) {
                // Integers need no formatting
                return $object;
            }
        }

        // Default to String format just to be safe.
        return '"' . $object . '"';
    }

    /**
     * Builds the SPARQL update query from the provided parameters.
     *
     * @param array $namespaces        Associative array of namespaces keyed by
     *                                 prefix
     * @param array $triples_to_remove Array of formatted triples to remove
     * @param array $triples_to_add    Array of formatted triples to add
     *
     * @return string The SPARQL update query
     */
    private function _buildQuery($namespaces, $triples_to_remove, $triples_to_add)
    {
        $query = "";

        // Prefixes
        foreach ($namespaces as $prefix => $namespace) {
            $query .= "PREFIX $prefix: <$namespace>\n";
        }

        // DELETE clause
        $query .= "\nDELETE WHERE {\n";

        foreach ($triples_to_remove as $triple) {
            $query .= "\t$triple\n";
        }

        $query .= "};\n";

        // INSERT CLAUSE
        $query .= "INSERT DATA {\n";

        foreach ($triples_to_add as $triple) {
            $query .= "\t$triple\n";
        }

        $query .= "}\n";

        return $query;
    }
}
