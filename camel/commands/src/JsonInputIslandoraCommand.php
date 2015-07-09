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
 * @package  Islandora
 * @author   Daniel Lamb <daniel@discoverygarden.ca>
 * @license  http://www.gnu.org/licenses/gpl-3.0.en.html GPL
 * @link     http://www.islandora.ca
 */
namespace Islandora;

use Islandora\IslandoraCommand;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;

/**
 * Islandora command that accepts JSON from STDIN.
 *
 * Serializes JSON into an array during initialization.
 *
 * @category Islandora
 * @package  Islandora
 * @author   Daniel Lamb <daniel@discoverygarden.ca>
 * @license  http://www.gnu.org/licenses/gpl-3.0.en.html GPL
 * @link     http://www.islandora.ca
 */
abstract class JsonInputIslandoraCommand extends IslandoraCommand
{
    /**
     * Initializes the command just after the input has been validated.
     *
     * Serializes JSON data from STDIN.
     *
     * @param InputInterface  $input  An InputInterface instance
     * @param OutputInterface $output An OutputInterface instance
     *
     * @return null
     */
    protected function initialize(InputInterface $input, OutputInterface $output)
    {
        parent::initialize($input, $output);
        $this->data = json_decode($this->data, true);
    }
}
