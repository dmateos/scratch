# Create your views here.
from django.http import HttpResponse, HttpResponseRedirect, Http404
from django.shortcuts import render_to_response, get_object_or_404
from django.views.decorators.cache import cache_page
from django.core.cache import cache

from urlhasher.models import URL, URLHitStat
from urlhasher.forms import URLSubmitForm

import hashlib

# Chars to split the hash at.
HASHLEN = 8

### Model writters ###

def write_url(url, requester):
    """Write the url entry to the database and return it, return
       the exsisting one if its already there. """

    hashobj = hashlib.sha1()
    hashobj.update(url)
    hash = hashobj.hexdigest()[:HASHLEN]

    # Maybe already exsists, cant hurt to try.
    try:
        match = URL.objects.get(hash=hash)
        return match
    # Oh noz, lets add it
    except:
        entry = URL(url=url, hash=hash, ipaddr=requester.META["REMOTE_ADDR"], hits=0)
        entry.save()
        return entry

def dump_user_info(request, hash):
    """Collect various session statistics and data from a django request object
       and dump it to a table"""

    ipaddr = request.META["REMOTE_ADDR"]
    userag = request.META["HTTP_USER_AGENT"]
    referer = request.META["HTTP_REFERER"]

    userdump = URLHitStat(hash=hash, ipaddr=ipaddr, useragent=userag, referer=referer)
    userdump.save()

### Views ###

def get_url(request, hash):

    # Get the URL and update its hit count.
    match = URL.objects.get(hash=hash)
    match.hits += 1
    match.save()

    # Dump users session info for statistics and redirect to url.
    dump_user_info(request, hash)
    return HttpResponseRedirect(match.url)

def push_url_post(request):
    hashed_url = "invalid url, fker :("
    hashed_urls = hashed_urls = URL.objects.all().order_by("pub_date").reverse()[:5]
    popular_urls = URL.objects.all().order_by("hits").reverse()[:5]
    form = URLSubmitForm()

    # Got post?
    if request.method == "POST":
        form = URLSubmitForm(request.POST)

        if form.is_valid():
            entry = write_url(form.cleaned_data["url"], request)
            hashed_url = entry.hash

    return render_to_response("result.html", locals())

def index(request):

    hashed_urls = URL.objects.all().order_by("pub_date").reverse()[:5]
    popular_urls = URL.objects.all().order_by("hits").reverse()[:5]
    form = URLSubmitForm()

    return render_to_response("base.html", locals())
