# Scraper - A Simple Web-Scraper made in C.

This is a simple Web-Scraper made in C using libcurl. 

It tries to find and extract all links on the site and then proceeds to put them in a file called `links.txt`.
Then it will try to follow the links that it found on the site an get the links from that site aswell.

**Compiling (with GCC):**
```bash
gcc scraper.c -o scraper -lcurl
```

**Usage:**

The user will be prompted for a http/https link: just paste on in there and you should be good to go.
