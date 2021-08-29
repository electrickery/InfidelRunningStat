# -*- coding: utf-8 -*-
from __future__ import absolute_import, unicode_literals

import octoprint.plugin

class InfidelRunningStatPlugin(octoprint.plugin.EventHandlerPlugin,
                               octoprint.plugin.StartupPlugin):

    infidelRSOnline = False

    def on_after_startup(self):
        self._logger.info("Infidel RunningStat demo plugin")

    # defining defaults for variables
    def get_settings_defaults(self):
        return {
            "infidelRSPort": "/dev/ttyUSB0",
            "infidelRSBaud": 9600,
        }

    # called on the subscribed events
    def on_event(self, event, payload):
        # see https://docs.octoprint.org/en/master/events/

	self._logger.info("Got event: '%s'", event)

        if (event == "Connected"):
	    self._logger.info("Got the CONNECTED event")
            self.connectInfidelRS()
            self.sendInfidelRS("C")

        if (event == "Disconnected"):
	    self._logger.info("Got the DISCONNECTED event")
            self.disconnectInfidelRS()

        if (event == "PrintStarted"):
            self._logger.info("Got the 'PrintStarted' event")
            self.sendInfidelRS("C")
            self.sendInfidelRS("S")

        if (event == "PrintFailed"):
            self._logger.info("Got the 'PrintFailed' event")
            self.sendInfidelRS("P")

        if (event == "PrintDone"):
            self._logger.info("Got the 'PrintDone' event")
            self.sendInfidelRS("P")

        if (event == "PrintCancelled"):
            self._logger.info("Got the 'PrintCancelled' event")
            self.sendInfidelRS("P")

        if (event == "PrintPaused"):
            self._logger.info("Got the 'PrintPaused' event")
            self.sendInfidelRS("P")

        if (event == "PrintResumed"):
            self._logger.info("Got the 'PrintResumed' event")
            self.sendInfidelRS("S")

    def connectInfidelRS(self):
        self._logger.info("Open port to InfidelRunningStat")
        # ....
        self.infidelRSOnline = True

    def disconnectInfidelRS(self):
        self._logger.info("Closing port to InfidelRunningStat")
        # ....
        self.infidelRSOnline = False

    def sendInfidelRS(self, msg):
        if (self.infidelRSOnline):
            self._logger.info("Sending %s to InfidelRunningStat", msg)
            # ....
        else:
	    self._logger.info("InfidelRunningStat port closed. '%s' not send.", msg)

__plugin_name__ = "InfidelRunningStatPlugin"
__plugin_version__ = "1.0.0"
__plugin_description__ = "Infidel RunningStat demo plugin for OctoPrint"
__plugin_pythoncompat__ = ">=2.7,<4"
__plugin_implementation__ = InfidelRunningStatPlugin()
