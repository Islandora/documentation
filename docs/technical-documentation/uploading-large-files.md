If you want to upload large (> 1GB) files, you'll need to tune the following settings in your `php.ini`:

- `upload_max_filesize` – The maximum allowed upload file size.
- `post_max_size` – The maximum allowed POST data size.
- `max_input_time` – Maximum allowed input time.
- `max_execution_time` – The maximum allowed time the scripts are allowed to run.

However, large file transfer over HTTP still has a host of issues once you properly configure your server.
Uploads are not resumable and subject to connectivity issues.
If you really want to upload large files, you should consider some alternatives such as

- Using the [TUS file upload protocol](https://www.drupal.org/project/tus) module, which will let you upload large files in forms.
- Using [flysystem](https://www.drupal.org/project/flysystem)'s ftp and sftp plugins to make files available if you can run an FTP server.
