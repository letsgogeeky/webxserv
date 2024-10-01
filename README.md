# Webserv

![](default/img/Webserv%20Lifecycle%20Overview.png)


# To pypass chrome CORS policy

Paste this in chrome and disable the attr: `chrome://flags/#block-insecure-private-network-requests`

reference: https://developer.chrome.com/blog/cors-rfc1918-feedback


# Benchmarking with siege
```bash
make docker-run
make re
./webserv
siege -b -c 100 -t 1m -T 2S http://127.0.0.1:8080/benchmark
```

# Curl with detailed reporting

let's agree, curl is cute
```bash
curl -v -s -w "\n-------\n\nDNS Lookup: %{time_namelookup}\nConnect: %{time_connect}\nStart Transfer: %{time_starttransfer}\nTotal: %{time_total}\n" http://localhost:8080/cgi-bin/blocking.py

curl -o /dev/null -s -w "DNS Lookup: %{time_namelookup}\nConnect: %{time_connect}\nStart Transfer: %{time_starttransfer}\nTotal: %{time_total}\n" http://localhost:8080/cgi-bin/blocking.py
```

### How I used git-filter-repo

1- Get an idea of what are the large files and if they have a pattern. I used these 2 queries

```bash
git rev-list --objects --all | \
git cat-file --batch-check='%(objectname) %(objecttype) %(rest)' | \
grep blob | \
sort -k3nr | \
head -n 100

```
```bash
git rev-list --objects --all | \
  grep -vF --file=<(git verify-pack -v .git/objects/pack/pack-* | \
  grep blob | \
  sort -k 3 -n | \
  tail -10 | \
  awk '{print$1}') | \
  cut -d' ' -f2- | \
  sort -k2 -n | \
  tail -50
```

2- Pick an individual file to annhilate.

`git-filter-repo --path webserv --invert-paths --force`
If you don't have `git-filter-repo` install it using `brew install git-filter-repo` or equivalent in your OS.

3- Running filter-repo will drop the remote path so you have to add it again.

`git remote add origin git@github.com:letsgogeeky/webxserv.git`

4- Compile and run to make sure nothing went wrong. run test cases and whatnot.

5- If you have a lot of files with a pattern to delete use this:

`git-filter-repo --path-glob '.ccls-cache/@@*' --invert-paths --force`

6- Add remote again if dropped, then force push all

`git push origin --force --all`