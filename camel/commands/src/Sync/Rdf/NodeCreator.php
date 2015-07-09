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

use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;

/**
 * Creates a Drupal node from Fedora RDF.
 *
 * @category Islandora
 * @package  Islandora\Sync\Rdf
 * @author   Daniel Lamb <daniel@discoverygarden.ca>
 * @license  http://www.gnu.org/licenses/gpl-3.0.en.html GPL
 * @link     http://www.islandora.ca
 */
class NodeCreator extends NodeUpdater
{
    /**
     * Sets command configuration.
     *
     * @return null
     */
    protected function configure()
    {
        $this->setName('rdf:createNode')
            ->setDescription(
                "Creates a Drupal node from Fedora RDF."
            );
    }

    /**
     * Creates a Drupal node from Fedora RDF.
     *
     * @param InputInterface  $input  An InputInterface instance
     * @param OutputInterface $output An OutputInterface instance
     *
     * @return null
     */
    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $rdf = &$this->data['rdf'][0];

        $node = array(
            'type' => $this->data['contentType'],
            'language' => 'und',
            'status' => '1',
            'promote' => '1',
            'uuid' => $this->data['uuid'],
            'rdf_mapping' => &$this->data['mappings']['rdf_mapping'],
            'rdf_namespaces' => &$this->data['mappings']['rdf_namespaces'],
        );

        $this->updateNodeFromRdf($node, $rdf);

        $output->write(json_encode($node));
    }

    /**
     * Generates a list of properties to ignore for this command.
     *
     * @return array
     */
    protected function generateIgnoreList()
    {
        return array(
                'rdftype',
                'body',
                'lastActivity',
        );
    }
}

