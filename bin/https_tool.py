#!/usr/bin/env python3

# openssl req -new -x509 -keyout localhost.pem -out localhost.pem -days 365 -nodes

import http.server
import ssl

def main():
    server_address = ('0.0.0.0', 8043)
    httpd = http.server.HTTPServer(server_address, http.server.SimpleHTTPRequestHandler)
    httpd.socket = ssl.wrap_socket(httpd.socket,
    server_side=True,
    certfile='localhost.pem',
    ssl_version=ssl.PROTOCOL_TLSv1_2)
    httpd.serve_forever()

if __name__ == '__main__':
    main()

