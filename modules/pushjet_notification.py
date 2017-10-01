import requests

from modules import NotificationInterface

class PushjetNotification(NotificationInterface):
    pj_secret = "96e515d24451024582e79119c061XXXX"

    def pushMessage(self, message):
        if len(self.pj_secret) < 12:
            print("no notification sent due to wrong pushjet secret.")
            return

        data = {
            "secret": str(self.pj_secret),
            "message": str(message),
            "title": "Camera Synchronization",
            "level": 1,
            "link": str("no web link")
        }

        r = requests.post('https://api.pushjet.io/message', data=data)

        if r.status_code == requests.codes.ok:
            print("successfuly sent notification!")
        else:
            print("couldn't send notification!")
            print("code:"+ str(r.status_code))
            print("headers:"+ str(r.headers))
            print("content:"+ str(r.text))