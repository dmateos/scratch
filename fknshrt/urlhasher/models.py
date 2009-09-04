from django.db import models
import datetime

class URL(models.Model):
    pub_date = models.DateTimeField("Publish date", auto_now=True)
    url = models.CharField("URL", max_length=255)
    hash = models.CharField("URL Hash", max_length=255)
    ipaddr = models.CharField("Submitter IP", max_length=12)
    hits = models.IntegerField("URL hits")

    def __unicode__(self):
        return self.url

class URLHitStat(models.Model):
    access_time = models.DateTimeField("Access time", auto_now=True)
    hash = models.CharField("URL Hash", max_length=255)
    ipaddr = models.CharField("IP", max_length=12)
    referer = models.CharField("Referer site", max_length=255)
    useragent = models.CharField("Users Browser", max_length=255)

    def __unicode__(self):
        return self.hash
