from django.conf.urls import patterns, include, url
from django.contrib import admin
from django.conf import settings
from django.conf.urls.static import static

from fb.views import (
message_person,

    index,
    post_details,
    login_view,
    logout_view,
    profile_view,
    edit_profile_view,
    like_view,
    group_view,
    create_new_group,
    friend_view,
    friend_request_view,
    friend_request_view_auto,
    dislike_view,
    delete_post,
    edit_post_view,
)


admin.autodiscover()

urlpatterns = patterns(
    '',
    url(r'^$', index, name='index'),
    url(r'^post/(?P<pk>\d)/$', post_details, name='post_details'),
    url(r'^group/(?P<pk>\d)/$', group_view, name='group_page'),
    url(r'^friends/(?P<pk>\d)/$', friend_view, name='friends_page'),
    url(r'^friendrequest/(?P<pk>\d)/$', friend_request_view, name='friend_page_request'),
    url(r'^friendrequestauto/(?P<pk>\d)/$', friend_request_view_auto, name='friend_page_request_auto'),
    url(r'^groups/$', create_new_group, name='all_groups'),
    url(r'^post/(?P<pk>\d)/like$', like_view, name='like'),
    url(r'^post/(?P<pk>\d)/dislike$', dislike_view, name='dislike'),
    url(r'^accounts/login/$', login_view, name='login'),
    url(r'^accounts/logout/$', logout_view, name='logout'),
    url(r'^profile/(?P<user>\w+)/$', profile_view, name='profile'),
    url(r'^profile/(?P<user>\w+)/edit$', edit_profile_view,
        name='edit_profile'),
    url(r'^message/$',message_person,name='message_someone'),
    url(r'^message/message_person$',message_person,name='message_person'),
    url(r'^admin/', include(admin.site.urls)),

    url(r'^post/(?P<pk>\d)/delete$', delete_post, name='delete_post'),
    url(r'^post/(?P<pk>\d)/edit$', edit_post_view, name='edit_post'),

) + static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)
