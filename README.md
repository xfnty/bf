# Tiny HTTP Server

References:
- https://github.com/suzusuzu/echo_server/tree/main
- https://man7.org/linux/man-pages/man2/select.2.html
- https://www.w3.org/Protocols/HTTP/1.0/spec.html#Response


## Todo
- [x] basic echo server
- [x] socket multiplexing using `select()`
- [x] HTTP request parsing
- [x] URI normalization
- [x] create HTTP response for HTML files only
- [x] serve static web pages
- [x] basic routing and route handlers
- [x] refactor and fix memory leaks
- [ ] implement `net_con_recv_chunks()` and `net_con_send_chunks()`
- [ ] create a decent HTTP response builder for different HTTP header fields and MIME types
- [ ] improve router (make `route_handler` a struct with function pointer and arguments)
- [ ] use `poll()` instead of `select()` for socket multiplexing
- [ ] add HTML template engine [1](https://gitlab.com/jobol/mustach), [2](https://github.com/rzeldent/micro-moustache)
- [ ] refactor API, make the project a framework (v1.0)


## Usage
```
./ths
```


## Developing
Setting up [pre-commit](https://pre-commit.com/):
```
pip install pre-commit
pre-commit install --hook-type pre-commit
pre-commit install --hook-type pre-push
```

Building and running:
```
./toolchain.py -cbr
```
