OVERVIEW
********

You can define your own configurations specific to your environment by copying
default.test_config.ini to test_config.ini, making your changes in the copied
file. These test need write access to the system's $FEDORA_HOME/server/config
directory as well as the filter-drupal.xml file.

DATASTREAM VALIDATION TESTS
***************************

The datastream validator included in the Islandora testing suite is able to
generate tests procedurally based on the files in the folder
'fixtures/datastream_validator_files'. By default, this folder is empty.
The unit tests for the validator pull the name of the file (before the
extension) and use that to instantiate the correct ______DatastreamValidator
class to test that file against (e.g. Image.jpg spins up an instance of the
ImageDatastreamValidator class and checks the results).

You can test against multiple different encodings of the same filetype by giving
each file a different set of extensions, e.g. MP3.vbr.mp3 and MP3.sbr.mp3 both
test against the MP3 datastream validator, even though both are encoded
differently.

For classes that require the third parameter (e.g. the TextDatastreamValidator),
place an additional name.extension.ini file in the datastream_validator_files
folder (e.g. the Text.txt would be paired with Text.txt.ini). This .ini file
should be structured like a PHP .ini file (e.g. according to the format used by
http://php.net/parse_ini_file).The generated test will parse the .ini
file as an array and pass it on to the third parameter.

The following prefixes are currently available for use:

- Image (jpg, png, gif, and other filetypes recognized by PHPGD)
- TIFF
- JP2
- PDF
- Text (requires a configured .ini)
- WAV
- MP3
- MP4
- OGG (asserts OGG video; use an .ini with an 'audio' key to test audio only)
- MKV
