// http2_servus.go - oraculum HTTP/2 pro probationibus (http2-plan C6).
//
// BIBLIOTHECA NORMA SOLA (D9): nullum go.mod, nullum golang.org/x/net.
// Solum a probationibus et tools/http2_tabulare.sh ut processus filius
// curritur, numquam nectitur. GODEBUG=http2debug=2 formas omnes in
// stderr scribit (tabulae pro fixis, http2-plan T9).
//
// Usus: go run tools/oracula/http2_servus.go -portus N -cert C.pem -clavis C.key
//
// Viae:
//   /salve          200 "salve"
//   /eco            corpus POST redditum
//   /magnum?n=N     N octeti ('a'..'z' iterati)
//   /tarde?ms=N     post N ms "tarde"
//   /abire          200 "vale", deinde Shutdown (GOAWAY omnibus)
package main

import (
	"context"
	"flag"
	"fmt"
	"io"
	"net/http"
	"os"
	"strconv"
	"time"
)

func main() {
	portus := flag.Int("portus", 0, "portus auscultandi (127.0.0.1)")
	cert := flag.String("cert", "", "certificatum PEM")
	clavis := flag.String("clavis", "", "clavis PEM")
	flag.Parse()

	mux := http.NewServeMux()
	servus := &http.Server{
		Addr:    fmt.Sprintf("127.0.0.1:%d", *portus),
		Handler: mux,
	}

	mux.HandleFunc("/salve", func(w http.ResponseWriter, r *http.Request) {
		w.Write([]byte("salve"))
	})
	mux.HandleFunc("/eco", func(w http.ResponseWriter, r *http.Request) {
		io.Copy(w, r.Body)
	})
	mux.HandleFunc("/magnum", func(w http.ResponseWriter, r *http.Request) {
		n, _ := strconv.Atoi(r.URL.Query().Get("n"))
		pars := make([]byte, 16384)
		for i := range pars {
			pars[i] = byte('a' + i%26)
		}
		for n > 0 {
			k := min(n, len(pars))
			w.Write(pars[:k])
			n -= k
		}
	})
	mux.HandleFunc("/tarde", func(w http.ResponseWriter, r *http.Request) {
		ms, _ := strconv.Atoi(r.URL.Query().Get("ms"))
		time.Sleep(time.Duration(ms) * time.Millisecond)
		w.Write([]byte("tarde"))
	})
	mux.HandleFunc("/abire", func(w http.ResponseWriter, r *http.Request) {
		w.Write([]byte("vale"))
		go func() {
			time.Sleep(50 * time.Millisecond)
			servus.Shutdown(context.Background())
		}()
	})

	err := servus.ListenAndServeTLS(*cert, *clavis)
	if err != nil && err != http.ErrServerClosed {
		fmt.Fprintln(os.Stderr, "http2_servus:", err)
		os.Exit(1)
	}
}
