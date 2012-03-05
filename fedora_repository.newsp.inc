<?php

function fedora_repository_newsp_page() 
{
    $solution_packs = array();
    $enabled_solution_packs = module_invoke_all('setup_defaults', $solution_packs);
    var_dump($solution_packs);
    var_dump($enabled_solution_packs);
}

?>
