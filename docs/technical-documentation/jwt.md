Islandora uses [JWT tokens](https://en.wikipedia.org/wiki/JSON_Web_Token) to authenticate communication between its components. RSA private public key pair is used to sign and verify JWT tokens. The process of issuing JWT tokens using RSA private key is handled by the Drupal [jwt](https://www.drupal.org/project/jwt) module.

The private public RSA pair needed by JWT authentication mechanism is generated in the web server. By default, claw playbook places 
the keys in `/opt/islandora/auth`.  Crayfish and Tomcat/Karaf need the public key to verify the JWT token. By default, they are put in the following locations: `/var/www/html/Crayfish/public.key`, `/etc/tomcat8/public.key`. If you are deploying Crayfish and Karaf/Tomcat components to different servers, ensure that web server public.key files are in the expected locations. 

Note that the connection need to be over SSL or an encrypted channel for this communication to be secure. Otherwise, a third party can capture your token and get access to your servers.

The JWT tokens expiration time is configurable via Islandora core settings: `http://localhost:8000/admin/config/islandora/core`. Currently it is recommend to set the `JWT Expiry` to the maximum expected time for a job, including batch jobs.
