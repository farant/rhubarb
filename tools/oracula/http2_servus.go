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
//
//	/salve          200 "salve"
//	/eco            corpus POST redditum
//	/magnum?n=N     N octeti ('a'..'z' iterati)
//	/tarde?ms=N     post N ms "tarde"
//	/abire          200 "vale", deinde Shutdown (GOAWAY omnibus)
//	/3/device/<s>   APNs fictum (http2-plan T11): capita accepta in
//	                x-thema, x-genus, x-auctoritas redduntur; sine
//	                "bearer " 403 MissingProviderToken; aliter 400
//	                BadDeviceToken - cum -modus iterare signum nullum
//	                400 DeviceTokenNotForTopic (regula iterandi).
//	                Praefixa signi (pulsus-plan T4, apns_mittens):
//	                c8 200; c410 410 Unregistered; c429 429 bis, deinde
//	                200; c403 403 ExpiredProviderToken semel, deinde 200;
//	                c500 semper 500. Petitio quaeque in stderr: "apns:
//	                signum= thema= genus= prioritas= expiratio= corpus="
package main

import (
	"context"
	"flag"
	"fmt"
	"io"
	"net/http"
	"os"
	"strconv"
	"strings"
	"sync"
	"time"
)

func main() {
	portus := flag.Int("portus", 0, "portus auscultandi (127.0.0.1)")
	cert := flag.String("cert", "", "certificatum PEM")
	clavis := flag.String("clavis", "", "clavis PEM")
	modus := flag.String("modus", "", "iterare: signum nullum -> DeviceTokenNotForTopic")
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

	var sera sync.Mutex
	visae := map[string]int{}
	mux.HandleFunc("/3/device/", func(w http.ResponseWriter, r *http.Request) {
		signum := strings.TrimPrefix(r.URL.Path, "/3/device/")
		auctoritas := r.Header.Get("authorization")
		corpus, _ := io.ReadAll(r.Body)
		fmt.Fprintf(os.Stderr, "apns: signum=%s thema=%s genus=%s prioritas=%s expiratio=%s corpus=%s\n",
			signum, r.Header.Get("apns-topic"), r.Header.Get("apns-push-type"),
			r.Header.Get("apns-priority"), r.Header.Get("apns-expiration"), corpus)
		sera.Lock()
		visae[signum]++
		vices := visae[signum]
		sera.Unlock()
		w.Header().Set("apns-id", "00000000-0000-0000-0000-000000000001")
		w.Header().Set("x-thema", r.Header.Get("apns-topic"))
		w.Header().Set("x-genus", r.Header.Get("apns-push-type"))
		w.Header().Set("x-auctoritas", auctoritas)
		w.Header().Set("content-type", "application/json")
		switch {
		case r.Method != http.MethodPost:
			w.WriteHeader(http.StatusMethodNotAllowed)
			w.Write([]byte(`{"reason":"MethodNotAllowed"}`))
		case !strings.HasPrefix(auctoritas, "bearer ") || len(auctoritas) == len("bearer "):
			w.WriteHeader(http.StatusForbidden)
			w.Write([]byte(`{"reason":"MissingProviderToken"}`))
		case strings.HasPrefix(signum, "c8"),
			strings.HasPrefix(signum, "c429") && vices > 2,
			strings.HasPrefix(signum, "c403") && vices > 1:
			w.WriteHeader(http.StatusOK)
		case strings.HasPrefix(signum, "c410"):
			w.WriteHeader(http.StatusGone)
			w.Write([]byte(`{"reason":"Unregistered"}`))
		case strings.HasPrefix(signum, "c429"):
			w.WriteHeader(http.StatusTooManyRequests)
			w.Write([]byte(`{"reason":"TooManyRequests"}`))
		case strings.HasPrefix(signum, "c403"):
			w.WriteHeader(http.StatusForbidden)
			w.Write([]byte(`{"reason":"ExpiredProviderToken"}`))
		case strings.HasPrefix(signum, "c500"):
			w.WriteHeader(http.StatusInternalServerError)
			w.Write([]byte(`{"reason":"InternalServerError"}`))
		case *modus == "iterare" && strings.Trim(signum, "0") == "":
			w.WriteHeader(http.StatusBadRequest)
			w.Write([]byte(`{"reason":"DeviceTokenNotForTopic"}`))
		default:
			w.WriteHeader(http.StatusBadRequest)
			w.Write([]byte(`{"reason":"BadDeviceToken"}`))
		}
	})

	err := servus.ListenAndServeTLS(*cert, *clavis)
	if err != nil && err != http.ErrServerClosed {
		fmt.Fprintln(os.Stderr, "http2_servus:", err)
		os.Exit(1)
	}
}
