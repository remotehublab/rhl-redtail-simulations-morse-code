#
# Copyright (C) 2023 onwards LabsLand, Inc.
# All rights reserved.
#
# This software is licensed as described in the file LICENSE, which
# you should have received as part of this distribution.
#
import os

class Config:
    SECRET_KEY = os.environ.get('SECRET_KEY')

    BINARY_DIRECTORY = os.environ.get('BINARY_DIRECTORY') or '../cmake-build-debug'
    SIMULATIONS_DIRECTORY = os.environ.get('SIMULATIONS_DIRECTORY') or 'simulations'
    SIMULATION_CONFIG_FILE = os.environ.get('SIMULATION_CONFIG_FILE')

class DevelopmentConfig(Config):
    DEBUG = True
    SECRET_KEY = 'secret'

class StagingConfig(Config):
    DEBUG = False

class ProductionConfig(Config):
    DEBUG = False

configurations = {
    'default': DevelopmentConfig,
    'development': DevelopmentConfig,
    'staging': StagingConfig,
    'production': ProductionConfig,
}
