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

use Symfony\Component\Console\Command\Command;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;

/**
 * Base class for Islandora commands.
 *
 * Gets message body from camel via STDIN during initialization.
 *
 * @category Islandora
 * @package  Islandora
 * @author   Daniel Lamb <daniel@discoverygarden.ca>
 * @license  http://www.gnu.org/licenses/gpl-3.0.en.html GPL
 * @link     http://www.islandora.ca
 */
abstract class IslandoraCommand extends Command
{
    protected $data;

    /**
     * Initializes the command just after the input has been validated.
     *
     * Sets $data by reading from STDIN.
     *
     * @param InputInterface  $input  An InputInterface instance
     * @param OutputInterface $output An OutputInterface instance
     *
     * @return null
     */
    protected function initialize(InputInterface $input, OutputInterface $output)
    {
        $this->data = '';
        while (!feof(STDIN)) {
            $this->data .= fread(STDIN, 8192);
        }
    }
}
