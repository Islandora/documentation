<?php
/**
 *
 * Copyright (C) 2009 Progress Software, Inc. All rights reserved.
 * http://fusesource.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 To successfully run this example, you must first start the broker with stomp+ssl enabled.
 You can do that by executing:
 $ ${ACTIVEMQ_HOME}/bin/activemq xbean:activemq-connectivity.xml
 Then you can execute this example with:
 $ php connectivity.php
*/
// include a library
// make a connection
try
{
$con = new Stomp("tcp://192.168.56.101:61613");
// connect
#// send a message to the queue

$msg = '<?xml version="1.0" encoding="UTF-8"?>
<entry xmlns="http://www.w3.org/2005/Atom" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:fedora-types="http://www.fedora.info/definitions/1/0/types/">
  <id>urn:uuid:19aab165-c920-40a1-a349-443f60f84567</id>
  <updated>2010-08-08T15:57:08.659Z</updated>
  <author>
    <name>umroymr2</name>
    <uri>http://192.168.56.101:8080/fedora</uri>
  </author>
  <title type="text">modifyDatastreamByValue</title>
  <category term="uofm:highResImage" scheme="fedora-types:pid" label="xsd:string"></category>
  <category term="ISLANDORACM" scheme="fedora-types:dsID" label="xsd:string"></category>
  <category term="" scheme="fedora-types:altIDs" label="fedora-types:ArrayOfString"></category>
  <category term="High Resolution Image" scheme="fedora-types:dsLabel" label="xsd:string"></category>
  <category term="" scheme="fedora-types:formatURI" label="xsd:string"></category>
  <category term="[OMITTED]" scheme="fedora-types:dsContent" label="xsd:base64Binary"></category>
  <category term="DISABLED" scheme="fedora-types:checksumType" label="xsd:string"></category>
  <category term="none" scheme="fedora-types:checksum" label="xsd:string"></category>
  <category term="Modified by Islandora API." scheme="fedora-types:logMessage" label="xsd:string"></category>
  <category term="false" scheme="fedora-types:force" label="xsd:boolean"></category>
  <summary type="text">uofm:highResImage</summary>
  <content type="text">2010-08-08T15:57:08.657Z</content>
  <category term="3.3" scheme="info:fedora/fedora-system:def/view#version"></category>
  <category term="info:fedora/fedora-system:ATOM-APIM-1.0" scheme="http://www.fedora.info/definitions/1/0/types/formatURI"></category>
</entry>';

$con->send("/queue/fedora.apim.update", $msg);

#echo "Sent message with body 'test'\n";
// subscribe to the queue
#$con->subscribe("/topic/fedora.apim.update");
// receive a message from the queue

#$msg = $con->readFrame();

// do what you want with the message
#if ( $msg != null) {
#    echo "Received message with body '$msg->body'\n";
#    // mark the message as received in the queue
#    $con->ack($msg);
#} else {
#    echo "Failed to receive a message\n";
#}

// disconnect
} catch (StompException $e)
{
	echo "StompException!";
	var_dump($e);
}
?>
