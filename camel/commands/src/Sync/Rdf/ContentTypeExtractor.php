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

use Islandora\JsonInputIslandoraCommand;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;

/**
 * Extracts a Drupal content type from Fedora RDF.
 *
 * @category Islandora
 * @package  Islandora\Sync\Rdf
 * @author   Daniel Lamb <daniel@discoverygarden.ca>
 * @license  http://www.gnu.org/licenses/gpl-3.0.en.html GPL
 * @link     http://www.islandora.ca
 */
class ContentTypeExtractor extends JsonInputIslandoraCommand
{
    /**
     * Sets command configuration.
     *
     * @return null
     */
    protected function configure()
    {
        $this->setName('rdf:extractContentType')
            ->setDescription(
                "Extracts a Drupal content type from Fedora RDF."
            );
    }

    /**
     * Extracts a Drupal content type from Fedora RDF.
     *
     * @param InputInterface  $input  An InputInterface instance
     * @param OutputInterface $output An OutputInterface instance
     *
     * @return null
     */
    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $types = $this->data[0]['@type'];

        foreach ($types as $type) {
            if (strpos($type, 'http://islandora.ca/ontology/v2/') !== false) {
                $output->write(
                    str_replace(
                        'http://islandora.ca/ontology/v2/',
                        'islandora_',
                        $type
                    )
                );
                exit(0);
            }
        }

        throw new Exception("Drupal content type could not be extracted from RDF\n" . json_encode($this->data));
    }
}
