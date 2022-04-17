module tlse

// TODO: Convert 'adapter.c' to V
import time
import net

const (
	tls_v12 = 0x0303
)

struct C.TLSContext {
	// TODO: Add fields
}

fn C.tls_create_context(is_server u8, version u16) &C.TLSContext
fn C.tls_established(context &C.TLSContext) int
fn C.tls_close_notify(context &C.TLSContext)

fn C.connect_socket(host &u8, port int) int
fn C.connect_tls(socket int, context &C.TLSContext) int
fn C.read_tls(socket int, context &C.TLSContext, buffer &u8, len int) int
fn C.write_tls(socket int, context &C.TLSContext, buffer &u8, len u32) int

struct SSLConn {
mut:
	ctx      &C.TLSContext
	handle   int
	duration time.Duration
}

pub fn (con SSLConn) read(mut buffer []u8) ?int {
	res := con.socket_read_into_ptr(&u8(buffer.data), buffer.len) ?
	return res
}

pub fn (con SSLConn) socket_read_into_ptr(buf_ptr &u8, len int) ?int {
	res := C.read_tls(con.handle, con.ctx, buf_ptr, len)
	return res
}

pub fn (con SSLConn) write(bytes []u8) ?int {
	if C.tls_established(con.ctx) == 0 {
		return error('Cannot write to an unestablished TLS connection')
	}
	return C.write_tls(con.handle, con.ctx, &u8(bytes.data), bytes.len)
}

pub fn (mut con SSLConn) connect(host string, port int) ?int {
	// TODO: Allow for custom TCP connections passed to this function; this currently does not work since V sockets are non-blocking
	socket := C.connect_socket(host.str, port)
	con.handle = socket
	con.duration = 30 * time.second
	con.ctx = C.tls_create_context(0, tlse.tls_v12)
	C.connect_tls(socket, con.ctx)
	return socket
}

pub fn (con SSLConn) shutdown() {
	C.tls_close_notify(con.ctx)
}

pub fn new_ssl_conn() &SSLConn {
	return &SSLConn{
		ctx: 0
	}
}

pub fn create_tcp_conn_from_handle(sockfd int) ?&net.TcpConn {
	return &net.TcpConn{
		sock: voidptr(sockfd)
		// TODO: Hack to get around the fact that net.TcpSocket is private
	}
}
