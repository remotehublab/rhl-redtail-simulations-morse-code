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

from config import configurations

def create_app(config_name: str = 'default'):

    # Based on Flasky https://github.com/miguelgrinberg/flasky
    app = Flask(__name__)
    app.config.from_object(configurations[config_name])

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
    dut2sim_data = app.config['SIMULATION_CONFIG']['gpios']['dut2sim']
    if dut2sim_data.get('number') is None:
        if dut2sim_data.get('labels') is None:
            dut2sim_data['number'] = 10
            dut2sim_data['labels'] = []
        else:
            dut2sim_data['number'] = len(dut2sim_data['labels'])
    else:
        if dut2sim_data.get('labels') is None:
            dut2sim_data['labels'] = []
        else:
            if len(dut2sim_data['labels']) != dut2sim_data['number']:
                raise Exception(f"In {simulation_config_full_path}, dut2sim declares labels and numbers with different size")

    app.config['SIMULATION_CONFIG']['gpios']['sim2dut'].setdefault('file', 'output-gpios.txt')
    sim2dut_data = app.config['SIMULATION_CONFIG']['gpios']['sim2dut']
    if sim2dut_data.get('number') is None:
        if sim2dut_data.get('labels') is None:
            sim2dut_data['number'] = 10
            sim2dut_data['labels'] = []
        else:
            sim2dut_data['number'] = len(sim2dut_data['labels'])
    else:
        if sim2dut_data.get('labels') is None:
            sim2dut_data['labels'] = []
        else:
            if len(sim2dut_data['labels']) != sim2dut_data['number']:
                raise Exception(f"In {simulation_config_full_path}, sim2dut declares labels and numbers with different size")
    app.config['SIMULATION_CONFIG']['messages']['web2sim'].setdefault('file', "input-messages.txt")
    app.config['SIMULATION_CONFIG']['messages']['sim2web'].setdefault('file', "output-messages.txt")

    if not app.config['SIMULATION_CONFIG'].get('iframe'):
        raise Exception("iframe is mandatory in any simulation .yml file") 

    # Register views
    from .views.main import main_blueprint

    app.register_blueprint(main_blueprint)

    return app
