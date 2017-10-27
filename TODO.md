# TODO

    * After each `malloc()`, check for success.
    * Deal with 404 errors.
    * Handle header fields in HTTP requests and responses.
    * There is currently a fixed limit to the number of bytes that an HTTP
      request can be. Change this.
    * Handle other methods (e.g. HEAD, TRACE)
    * Compression (gzip)
    * Check response against request headers, e.g. 'Accept' vs. 'Content-Type'
