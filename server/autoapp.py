#
# Copyright (C) 2023 onwards LabsLand, Inc.
# All rights reserved.
#
# This software is licensed as described in the file LICENSE, which
# you should have received as part of this distribution.
#

import os
from hybridapi import create_app
application = create_app(os.environ.get('FLASK_CONFIG') or 'default')
