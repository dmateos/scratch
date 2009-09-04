from django.contrib import admin
from urlhasher.models import URL, URLHitStat

admin.site.register(URL)
admin.site.register(URLHitStat)
