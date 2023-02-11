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

    GPIO_WEB2SIMULATIONS_FILE = os.environ.get('GPIO_WEB2SIMULATIONS_FILE') or '../cmake-build-debug/input-gpios.txt'
    GPIO_SIMULATIONS2WEB_FILE_SIZE = int(os.environ.get('GPIO_SIMULATIONS2WEB_FILE_SIZE') or '10')
    GPIO_SIMULATIONS2WEB_FILE = os.environ.get('GPIO_SIMULATIONS2WEB_FILE') or '../cmake-build-debug/output-gpios.txt'
    GPIO_WEB2SIMULATIONS_FILE_SIZE = int(os.environ.get('GPIO_WEB2SIMULATIONS_FILE_SIZE') or '10')

    MESSAGES_WEB2SIMULATIONS_FILE = os.environ.get('MESSAGES_WEB2SIMULATIONS_FILE') or '../cmake-build-debug/input-messages.txt'
    MESSAGES_SIMULATIONS2WEB_FILE = os.environ.get('MESSAGES_SIMULATIONS2WEB_FILE') or '../cmake-build-debug/output-messages.txt'
   

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
