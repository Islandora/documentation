<?php

require '../vendor/autoload.php';

use hanneskod\classtools\Iterator\ClassIterator;
use Islandora\IslandoraCommand;
use Symfony\Component\Console\Application;
use Symfony\Component\Console\ConsoleEvents;
use Symfony\Component\Console\Event\ConsoleExceptionEvent;
use Symfony\Component\Console\Output\ConsoleOutputInterface;
use Symfony\Component\EventDispatcher\EventDispatcher;
use Symfony\Component\Finder\Finder;

// Set the error handler to wrap errors as exceptions.
set_error_handler(function ($errno, $errstr, $errfile, $errline, array $errcontext) {
    throw new ErrorException($errstr, 1, $errno, $errfile, $errline);
});

try {
    $dispatcher = new EventDispatcher();

    // This is neccessary to integrate with Symfony Console's exception
    // handling.  The global try/catch will never trigger if the exception
    // occurs inside a Command since Symfony does some squashing at the end of
    // execution.  
    $dispatcher->addListener(ConsoleEvents::EXCEPTION, function (ConsoleExceptionEvent $event) {
        $std_err = $event->getOutput();
        if ($std_err instanceof ConsoleOutputInterface)
        {
            $std_err = $std_err->getErrorOutput();
        }
        $std_err->writeln("\n" . $event->getException()->getMessage());
        $std_err->writeln("\n" . $event->getException()->getTraceAsString());
    });

    $application = new Application('Islandora Command Tool', '0.0.1-SNAPSHOT');
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
} catch (Exception $e) {
    // Write output to stderr when there's an exception outside of commmand
    // execution and exit with return code 1. 
    fwrite(STDERR, "\n{$e->getMessage()}\n");
    fwrite(STDERR, "\n{$e->getTraceAsString()}\n");
    exit(1);
}
