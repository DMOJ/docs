#Notes   
This tutorial is based on my experience installing DMOJ Django based site. This guide is intended for linux machines. Might work on other systems, but I offer no guarantee.   
This tutorial is supposed to work on a Ubuntu machine.

#Requirements
* Python 2
* pip
* git

#Step 1
Install virtualenv by using the following command 'sudo pip install virtualenv'

#Step 2
Create a virtualenv by using the following command 'virtualenv env'.
This environment will be enabled using the following command in the folder you ran 'virtualenv env': 'source env/bin/activate'

Now the command line should look like this:

'''
(env)walle256@server ~:
'''

#Step 3
Run: 'git clone https://github.com/DMOJ/site' and then 'cd site'

#Step 4
Run: 'pip install -r requirements.txt'
This step is a bit tricky, usually, you don't have all the libs that are required, but, for example if you don't have 'Python.h', run 'apt-get install python-dev'.
In this step, Google is your best friend.

#Step 5
Coming soon.
