<?php
/**
 * Created by PhpStorm.
 * User: whikloj
 * Date: 2016-07-25
 * Time: 2:11 PM
 */

namespace Islandora\CLAW;

use GuzzleHttp\Client;
use Islandora\Chullo\Uuid\UuidGenerator;
use Symfony\Component\Yaml\Yaml;

class StackTests extends \PHPUnit_Framework_TestCase
{

  protected $client;

  protected $config;

  protected $claw_address;

  protected $uuid_gen;

  protected $twig;

  public function __construct()
  {
    if (!file_exists('./config/settings.yml')) {
      throw new \Exception("Missing ./config/settings.yml");
    }
    $this->config = Yaml::parse(file_get_contents('./config/settings.yml'));

    $client = new \stdClass();

    // Define separate clients for Claw and Fedora
    $base_uri = $this->generateUri($this->config['claw']);
    $client->claw = new Client(['base_uri' => $base_uri]);

    $base_uri = $this->generateUri($this->config['fedora']);
    $client->fedora = new Client(['base_uri' => $base_uri]);

    $this->client = $client;

    $this->uuid_gen = new UuidGenerator();

    $loader = new \Twig_Loader_Filesystem(array(
      __DIR__ . '/../templates'
    ));
    $this->twig = new \Twig_Environment($loader, array(
      'cache' => false,
    ));
  }

  protected function deleteFedoraResource($uri)
  {
    $response = $this->client->fedora->request('GET', $uri, [
      'http_errors' => false
    ]);
    if ($response->getStatusCode() == 200) {
      $r = $this->client->fedora->request('DELETE', $uri);
      $this->assertEquals($r->getStatusCode(), 204, "Failed to delete $uri");
      $response = $this->client->fedora->request('GET', $uri, [
        'http_errors' => false
      ]);
    }
    if ($response->getStatusCode() == 410) {
      $uri .= '/fcr:tombstone';
      $this->client->fedora->request('DELETE', $uri);
    }
  }

  private function generateUri($config)
  {

    if (isset($config['protocol'])) {
      $tmp_uri = $config['protocol'];
    } else {
      // Assume http
      $tmp_uri = "http";
    }
    $tmp_uri .= "://";
    if (isset($config['host'])) {
      $tmp_uri .= $config['host'];
    } else {
      throw new \Exception("Missing configuration parameter 'host'");
    }
    $tmp_uri .= ":";
    if (isset($config['port'])) {
      $tmp_uri .= $config['port'];
    } else {
      throw new \Exception("Missing configuration parameter 'port'");
    }
    if (isset($config['path']) && !empty($config['path'])) {
      $path = ltrim($config['path'], '/');
      $tmp_uri .= '/' . $path;
    }
    return $tmp_uri;
  }
}
