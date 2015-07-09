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

use Islandora\JsonInputIslandoraCommand;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;

/**
 * Islandora command that can escape namespace prefixes.
 *
 * @category Islandora
 * @package  Islandora
 * @author   Daniel Lamb <daniel@discoverygarden.ca>
 * @license  http://www.gnu.org/licenses/gpl-3.0.en.html GPL
 * @link     http://www.islandora.ca
 */
abstract class PrefixEscapingCommand extends JsonInputIslandoraCommand
{
    /**
     * Escapes namespace prefixes in predicates.
     *
     * @param string $predicate  The predicate whose namespace you wish to escape
     * @param array  $namespaces Associative array of namespaces keyed by prefix
     *
     * @return string The predicate with escaped namespace prefix
     */
    protected function escapePrefix($predicate, array $namespaces)
    {
        $exploded = explode(":", $predicate);
        if (!isset($namespaces[$exploded[0]])) {
            return $predicate;
        }
        return $namespaces[$exploded[0]] . $exploded[1];
    }

}

