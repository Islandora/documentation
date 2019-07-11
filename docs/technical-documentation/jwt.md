Islandora uses [JWT tokens](https://en.wikipedia.org/wiki/JSON_Web_Token) to authenticate communication between its components. RSA private public key pair is used to sign and verify JWT tokens. The process of issuing JWT tokens using RSA private key is handled by the Drupal jwt module.

The private public RSA pair needed by JWT authentication mechanism is generated in the webserver and placed in `/opt/islandora/auth`. The public key needed to verify the JWT token is put at `/var/www/html/Crayfish/public.key` for Crayfish and `/etc/tomcat8/public.key` for Tomcat/Karaf. If you are deploying Crayfish and Karaf/Tomcat components to different servers, ensure that web server public.key files are in the expected locations. 

Note that communication need to be over SSL for this communication to be secure. Otherwise, a third party can capture your token and get access to your servers.

The JWT tokens expiration time is configurable via Islandora core settings: `http://localhost:8000/admin/config/islandora/core`. Currently it is recommend that JWT Expiry to be set the maximum expected time for a job, including batch jobs.
