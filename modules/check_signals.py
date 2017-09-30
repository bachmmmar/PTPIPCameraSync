import signal

class CheckSignals:

    def __init__(self):
        self._signal_received = False
        signal.signal(signal.SIGINT, self._exit_gracefully)
        signal.signal(signal.SIGTERM, self._exit_gracefully)

    def terminationSignalReceived(self):
        return self._signal_received

    def _exit_gracefully(self,signum, frame):
        print('Termination signal received. Waiting for running tasks...')
        self._signal_received = True