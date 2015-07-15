<?php

require '../vendor/autoload.php';

use hanneskod\classtools\Iterator\ClassIterator;
use Islandora\IslandoraCommand;
use Symfony\Component\Console\Application;
use Symfony\Component\Console\ConsoleEvents;
use Symfony\Component\Console\Event\ConsoleCommandEvent;
use Symfony\Component\Console\Event\ConsoleExceptionEvent;
use Symfony\Component\Console\Output\ConsoleOutputInterface;
use Symfony\Component\EventDispatcher\EventDispatcher;
use Symfony\Component\Finder\Finder;

$dispatcher = new EventDispatcher();

// Set the error handler to wrap errors as exceptions at the beginning of
// every command.
$dispatcher->addListener(ConsoleEvents::COMMAND, function (ConsoleCommandEvent $event) {
    set_error_handler(function (int $errno, string $errstr, string $errfile, int $errline, array $errcontext) {
        throw new ErrorException($errstr, 1, $errno, $errfile, $errline);
    });
});

// Write output to stderr (if available) when there's an exception during
// command execution.
$dispatcher->addListener(ConsoleEvents::EXCEPTION, function (ConsoleExceptionEvent $event) {
    $std_err = $event->getOutput();

    if ($std_err instanceof ConsoleOutputInterface)
    {
        $std_err = $std_err->getErrorOutput();
    }

    $std_err->writeln($event->getException()->getMessage());
    $std_err->writeln($event->getException()->getTraceAsString());
});

$application = new Application();
$application->setDispatcher($dispatcher);

// A little magic to find all IslandoraCommand classes and dynamically
// instantiate them for the Symfony Console Application.
$finder = new Finder();
$iter = new ClassIterator($finder->in('../src'));
$iter->enableAutoloading();

foreach ($iter->type('Islandora\IslandoraCommand')->where('isInstantiable') as $class) {
    $class_name = $class->getName();
    $application->add(new $class_name());
}

// Run the command
$application->run();
