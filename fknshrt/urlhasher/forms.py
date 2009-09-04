from django import forms

class URLSubmitForm(forms.Form):
    url = forms.URLField(required=True)
