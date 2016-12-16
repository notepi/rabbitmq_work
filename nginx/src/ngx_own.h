#ifndef _NGX_OWN_H_INCLUDED_
#define _NGX_OWN_H_INCLUDED_

#define ngx_memzero(buf, n)       (void) memset(buf, 0, n)
#define ngx_memset(buf, c, n)     (void) memset(buf, c, n)

#endif/* _NGX_OWN_H_INCLUDED_ */
