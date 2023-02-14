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
import yaml
import random
import string
import getpass
import hashlib

from flask import Flask

# from flask_assets import Environment

from config import configurations

# assets = Environment()

def create_app(config_name: str = 'default'):

    # Based on Flasky https://github.com/miguelgrinberg/flasky
    app = Flask(__name__)
    app.config.from_object(configurations[config_name])

    # assets.init_app(app)

    # from .bundles import register_bundles
    # register_bundles(assets)

    if app.config['SIMULATION_CONFIG_FILE'] is None:
        raise Exception("Missing SIMULATION_CONFIG_FILE")

    simulation_config_full_path = os.path.join(app.config['SIMULATIONS_DIRECTORY'], app.config['SIMULATION_CONFIG_FILE'])
    if not os.path.exists(simulation_config_full_path):
        raise Exception(f"Simulation config file {simulation_config_full_path} not found. Change SIMULATION_CONFIG_FILE or SIMULATIONS_DIRECTORY")

    simulation_config = yaml.load(open(simulation_config_full_path), Loader=yaml.FullLoader)
    app.config['SIMULATION_CONFIG'] = simulation_config
    app.config['SIMULATION_CONFIG'].setdefault('gpios', {})
    app.config['SIMULATION_CONFIG'].setdefault('messages', {})
    app.config['SIMULATION_CONFIG']['gpios'].setdefault('sim2dut', {})
    app.config['SIMULATION_CONFIG']['gpios'].setdefault('dut2sim', {})
    app.config['SIMULATION_CONFIG']['messages'].setdefault('web2sim', {})
    app.config['SIMULATION_CONFIG']['messages'].setdefault('sim2web', {})

    app.config['SIMULATION_CONFIG'].setdefault('name', "Simulation")
    app.config['SIMULATION_CONFIG'].setdefault('description', "Simulation description")
    app.config['SIMULATION_CONFIG']['gpios']['dut2sim'].setdefault('file', 'input-gpios.txt')
    app.config['SIMULATION_CONFIG']['gpios']['dut2sim'].setdefault('number', 10)
    app.config['SIMULATION_CONFIG']['gpios']['sim2dut'].setdefault('file', 'output-gpios.txt')
    app.config['SIMULATION_CONFIG']['gpios']['sim2dut'].setdefault('number', 10)
    app.config['SIMULATION_CONFIG']['messages']['web2sim'].setdefault('file', "input-messages.txt")
    app.config['SIMULATION_CONFIG']['messages']['sim2web'].setdefault('file', "output-messages.txt")

    if not app.config['SIMULATION_CONFIG'].get('iframe'):
        raise Exception("iframe is mandatory in any simulation .yml file") 

    # Register views
    from .views.main import main_blueprint

    app.register_blueprint(main_blueprint)

    return app
