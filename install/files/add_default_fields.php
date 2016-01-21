<?php

module_load_include('inc', 'islandora_dc', 'include/fields');
try {
    islandora_dc_add_fields_to_bundle(ISLANDORA_BASIC_IMAGE_CONTENT_TYPE);
    islandora_dc_add_fields_to_bundle(ISLANDORA_BASIC_IMAGE_CONTENT_TYPE);
}
catch (Exception $e) {
    error_log($e);
}
