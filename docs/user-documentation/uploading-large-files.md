# Large Files

## Large Files and Drupal

If you want to upload large (> 1GB) files, you'll need to tune the following settings in your `php.ini`:

- `upload_max_filesize` – The maximum allowed upload file size.
- `post_max_size` – The maximum allowed POST data size.
- `max_input_time` – Maximum allowed input time.
- `max_execution_time` – The maximum allowed time the scripts are allowed to run.
- `default_socket_timeout` - Default timeout (in seconds) for socket based streams.

However, large file transfer over HTTP still has a host of issues once you properly configure your server.
Uploads are not resumable and subject to connectivity issues.
If you really want to upload large files, you should consider some alternatives such as

- Using the [TUS file upload protocol](https://www.drupal.org/project/tus) module, which will let you upload large files in forms.
- Using [flysystem](https://www.drupal.org/project/flysystem)'s ftp and sftp plugins to make files available if you can run an FTP server.

## Large Files and Fedora

If loading large (e.g. range 30-45 GB) files into Fedora, you may need to change the 
`fcrepo.session.timeout` property, which defaults to 3 minutes (180,000 ms). Documentation is on the
[Properties page on the Fedora wiki](https://wiki.lyrasis.org/display/FEDORAM6M1P0/Properties).

## Large Files and FITS

If using FITS, you may need to change the following in `/var/lib/tomcat9/webapps/fits/WEB-INF/classes/fits-service.properties`:

```
# Maximum allowable size of uploaded file
max.upload.file.size.MB=2000
# Maximum size of HTTP Request object. Must be equal to or larger than the value for max.upload.file.size.MB
max.request.size.MB=2000
# Maximum size of an uploaded file kept in memory. Otherwise temporarily persisted to disk.
max.in.memory.file.size.MB=4
```

