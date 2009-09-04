from django.conf.urls.defaults import *
from django.conf import settings
from django.contrib import admin
import os

# Uncomment the next two lines to enable the admin:
# from django.contrib import admin
admin.autodiscover()

urlpatterns = []

if not settings.PRODUCTION:
    urlpatterns += patterns('',
        (r'^static/(?P<path>.*)$', 'django.views.static.serve', {'document_root': os.path.join(settings.PROJECT_ROOT, "static/")})
    )

urlpatterns += patterns('',
    (r'^admin/(.*)', admin.site.root),

    # Fknshrt URL shortner.
    (r'^$', 'urlhasher.views.index'), # Main page.
    (r'^push_url_post$', 'urlhasher.views.push_url_post'),
    (r'^(.*)$', 'urlhasher.views.get_url'), # Redirect to hashed url, at end to catch all
)


