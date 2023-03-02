import boto3
from kivy.app import App
from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.lang import Builder

import os

s3 = boto3.resource('s3')

class MyWidget(BoxLayout):
    def open(self, path, filename):
        with open(os.path.join(path, filename[0])) as f:
            data = open(f.name, 'rb')
            s3.Bucket('test-aws-bucket-5790').put_object(Key=f.name, Body=data)

    def selected(self, filename):
        print("selected: %s" % filename[0])


class Editor(App):
    def build(self):
        return MyWidget()

if __name__ == '__main__':
    Editor().run()
