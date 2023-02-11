#
# Copyright (C) 2023 onwards LabsLand, Inc.
# All rights reserved.
#
# This software is licensed as described in the file LICENSE, which
# you should have received as part of this distribution.
#

import os
import sys
import json
import random
import string
import getpass
import hashlib

from flask import Flask
from flask_redis import FlaskRedis

from config import configurations

# Plugins
redis_store = FlaskRedis(decode_responses=True)

def create_app(config_name: str = 'default'):

    # Based on Flasky https://github.com/miguelgrinberg/flasky
    app = Flask(__name__)
    app.config.from_object(configurations[config_name])

    if config_name in ['development', 'default'] and '--with-threads' not in sys.argv and 'run' in sys.argv:
        print("***********************************************************")
        print("*                                                         *")
        print("*               I M P O R T A N T                         *")
        print("*                                                         *")
        print("* You must pass --with-threads when running this script   *")
        print("*                                                         *")
        print("***********************************************************")
        os._exit(-1)
    
    # Initialize plugins
    redis_store.init_app(app)

    # Register views
    from .views.main import main_blueprint

    app.register_blueprint(main_blueprint)

    return app
