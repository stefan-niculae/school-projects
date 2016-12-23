from django.core.urlresolvers import reverse
from django.shortcuts import render, redirect
from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.decorators import login_required
from django.core.files.uploadedfile import SimpleUploadedFile
from django.http import HttpResponseForbidden
from datetime import datetime,timedelta
from django.contrib.auth.models import User



from fb.models import UserPost, UserMessage, UserPostComment, UserProfile, GroupPost, Group, PostBase
from fb.forms import (

    UserPostForm, UserPostCommentForm, UserLogin, UserProfileForm, GroupUserPostForm, GroupsForm, EditPostForm

)


@login_required
def index(request):
    posts = PostBase.objects.all()
    if request.method == 'GET':
        form = UserPostForm()
    elif request.method == 'POST':
        form = UserPostForm(request.POST)
        if form.is_valid():
            text = form.cleaned_data['text']
            post = UserPost(text=text, author=request.user, usertype='user_post')
            post.group_post = None;
            post.save()

    context = {
        'posts': posts,
        'form': form,
    }
    return render(request, 'index.html', context)

@login_required
def delete_post(request, pk):
    UserPost.objects.get(pk=pk).delete()
    return redirect('/')

@login_required
def create_new_group(request):
    groups = Group.objects.all()
    # user = request.user
    # groups = user.group_members

    if request.method == 'GET':
        form = GroupsForm()
    elif request.method == 'POST':
        form = GroupsForm(request.POST)
        if form.is_valid():

            name = form.cleaned_data['text']

            group_exists = False

            for group in groups:
                if group.name == name:
                    group_exists = True
                    break

            if group_exists == False:
                group = Group(name=name, creator=request.user)
                group.save()

    context = {
        'groups': groups,
        'form': form,
    }
    return render(request, 'groups.html', context)


@login_required
def post_details(request, pk):
    post = UserPost.objects.get(pk=pk)

    if request.method == 'GET':
        form = UserPostCommentForm()
    elif request.method == 'POST':
        form = UserPostCommentForm(request.POST)
        if form.is_valid():
            cleaned_data = form.cleaned_data
            comment = UserPostComment(text=cleaned_data['text'],
                                      post=post,
                                      author=request.user)
            comment.save()

    comments = UserPostComment.objects.filter(post=post)

    context = {
        'post': post,
        'comments': comments,
        'form': form,
    }

    return render(request, 'post_details.html', context)


def login_view(request):
    if request.method == 'GET':
        login_form = UserLogin()
        context = {
            'form': login_form,
        }
        return render(request, 'login.html', context)
    if request.method == 'POST':
        login_form = UserLogin(request.POST)
        username = request.POST['username']
        password = request.POST['password']
        user = authenticate(username=username, password=password)
        if user is not None:
            login(request, user)
            return redirect('/')
        else:
            context = {
                'form': login_form,
                'message': 'Wrong user and/or password!',
            }
            return render(request, 'login.html', context)


@login_required
def logout_view(request):
    logout(request)
    return redirect(reverse('login'))


@login_required
def profile_view(request, user):
    profile = UserProfile.objects.get(user__username=user)
    context = {
        'profile': profile,
    }
    return render(request, 'profile.html', context)


@login_required
def edit_profile_view(request, user):
    profile = UserProfile.objects.get(user__username=user)
    if not request.user == profile.user:
        return HttpResponseForbidden()
    if request.method == 'GET':
        data = {
            'first_name': profile.user.first_name,
            'last_name': profile.user.last_name,
            'gender': profile.gender,
            'date_of_birth': profile.date_of_birth,
        }
        avatar = SimpleUploadedFile(
            profile.avatar.name, profile.avatar.file.read()) \
            if profile.avatar else None
        file_data = {'avatar': avatar}
        form = UserProfileForm(data, file_data)
    elif request.method == 'POST':
        form = UserProfileForm(request.POST, request.FILES)
        if form.is_valid():
            profile.user.first_name = form.cleaned_data['first_name']
            profile.user.last_name = form.cleaned_data['last_name']
            profile.user.save()

            profile.gender = form.cleaned_data['gender']
            profile.date_of_birth = form.cleaned_data['date_of_birth']
            if form.cleaned_data['avatar']:
                profile.avatar = form.cleaned_data['avatar']
            profile.save()

            return redirect(reverse('profile', args=[profile.user.username]))
    context = {
        'form': form,
        'profile': profile,
    }
    return render(request, 'edit_profile.html', context)


@login_required
def like_view(request, pk):
    post = UserPost.objects.get(pk=pk)

    #if not post.likers.get(pk=request.user.pk):
    if not request.user in post.likers.all():
        post.likers.add(request.user)
        if request.user in post.dislikers.all():
            post.dislikers.remove(request.user)
    else:
        post.likers.remove(request.user)

    post.save()
    return redirect(reverse('post_details', args=[post.pk]))

@login_required
def message_person(request):
    
    messages_sent = []
    messages_received = []
    messages=[]

    to=request.user.username
    copy=request.user.username
    if request.method == 'POST':
        form = (request.POST)
        to = form['to']
        message = form['message']
        try:
            user_target=User.objects.get(username=to)
        except User.DoesNotExist:
            to="Undefined User"

        if to!="Undefined User":
            post = UserMessage(text=message, sender=request.user,
                           receiver=user_target)
            post.save()

    if to!="Undefined User":
        copy="Messages with " + to
        
        messages_sent_db = UserMessage.objects.filter(
            sender__username=request.user.username, receiver__username=to)
        
        messages_received_db = UserMessage.objects.filter(
            receiver__username=request.user.username, sender__username=to)

        messages_sent = [m for m in messages_sent_db]
        messages_received = [m for m in messages_received_db]
        
        messages=sorted(messages_sent+messages_received,key=lambda s: s.date_added.replace(tzinfo=None))
    
    
    context = {
        'messages': messages,
        'message_target':copy,
        'to':to
    }
    
    return render(request, 'message.html', context)

def dislike_view(request, pk):
    post = UserPost.objects.get(pk=pk)

    if not request.user in post.dislikers.all():
        post.dislikers.add(request.user)
        if request.user in post.likers.all():
            post.likers.remove(request.user)
    else:
        post.dislikers.remove(request.user)

    post.save()
    return redirect(reverse('post_details', args=[post.pk]))

@login_required
def group_view(request, pk):
    group = Group.objects.get(pk=pk)
    posts = group.group_posts.all()

    if request.method == 'GET':
        form = GroupUserPostForm()
    elif request.method == 'POST':
        form = GroupUserPostForm(request.POST)
        if form.is_valid():
            text = form.cleaned_data['text']
            post = GroupPost(text=text, author=request.user, usertype='group_post')
            post.group_post = group
            post.save()

    context = {
        'group_posts': posts,
        'group': group,
        'form': form,
    }
    return render(request, 'group.html', context)


@login_required
def friend_view(request, pk):
    userr = UserProfile.objects.get(pk=pk)
    users = UserProfile.objects.all()
    pending = []

    # for user in request.user.friends.all():
    #     for x in user.user.friends.all():
    #         if x.user == request.user:
    #             pending.append(user.user);
    for user in request.user.friends.all():
        for x in user.user.friends.all():
            if x.user == request.user:
                pending.append(user.user);


    context = {
        'users': users,
        'current_user': request.user,
        'friends': request.user.friends.all(),
        'pending': pending,
        'non_friends': [x for x in users if x not in request.user.friends.all() and x != userr],
    }
    return render(request, 'friends.html', context)


@login_required
def friend_request_view(request, pk):

    user = UserProfile.objects.get(pk=pk)
    if(user != request.user.profile):
        request.user.friends.add(user)
    request.user.save()

    return redirect('/')

@login_required
def friend_request_view_auto(request, pk):

    user = UserProfile.objects.get(pk=pk)
    if(user != request.user.profile):
        request.user.friends.add(user)
    UserProfile.objects.get(pk=pk).friends.add(request.user)
    UserProfile.objects.get(pk=pk).save()
    request.user.save()

    return redirect('/')



@login_required
def edit_post_view(request, pk):

    post = UserPost.objects.get(pk=pk)
    form = EditPostForm(initial={
        'text': post.text}
    )

    if request.method == 'GET':

        post.save()

        context = {
            'form': form,
        }

        return render(request, 'edit_post.html', context)

    else:
        form = EditPostForm(request.POST)
        if form.is_valid():
            text = form.cleaned_data['text']
            #post = UserPost(text=text, author=request.user, usertype='user_post')
            post.text = text
            post.group_post = None;
            post.save()
        return redirect('/')
