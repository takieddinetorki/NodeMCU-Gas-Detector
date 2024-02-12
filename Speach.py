import speech_recognition as sr  # recognise speech
import playsound  # to play an audio file
from gtts import gTTS  # google text to speech
import random
from time import ctime  # get time details
import time
import os  # to remove created audio files
from PIL import Image
import pyttsx3
import bs4 as bs
import serial

ser = serial.Serial("COM6", 115200)


def your_speech_exists(terms):
    for term in terms:
        if term in voice_data:
            return True


def AI_Speaking(text):
    text = str(text)
    AI.say(text)
    AI.runAndWait()


r = sr.Recognizer()  # initialise a recogniser


# listen for audio and convert it to text:
def Audio_record(ask=""):
    with sr.Microphone() as source:  # microphone as source
        if ask:
            AI_Speaking(ask)
        audio = r.listen(source, 5, 5)  # listen for the audio via source
        print("Done Listening")
        voice_data = ''
        try:
            voice_data = r.recognize_google(audio)  # convert audio to text
        except sr.UnknownValueError:  # error: recognizer does not understand
            print('Sorry, Try again!')
        except sr.RequestError:
            # error: recognizer is not connected
            print('Sorry, the service is down')
        print(">>", voice_data.lower())  # print what user said
        return voice_data.lower()


# get string and make a audio file to be played


def AI_Speaking(audio_string):
    audio_string = str(audio_string)
    tts = gTTS(text=audio_string, lang='en')  # text to speech(voice)
    r = random.randint(1, 30)
    audio_file = 'audio' + str(r) + '.mp3'
    tts.save(audio_file)  # save as mp3
    playsound.playsound(audio_file)  # play the audio file
    os.remove(audio_file)  # remove audio file


def respond(voice_data):
    if your_speech_exists(["on"]):
        print("Turning the fan on")
        AI_Speaking("Turning the fan on")
        ser.write(b'ON')
    if your_speech_exists(["off"]):
        AI_Speaking("Turning the fan off")
        print("Turning the fan on")
        ser.write(b'OFF')
    if your_speech_exists(["exit", "quit", "stop"]):
        AI_Speaking("Stopping")
        ser.close()
        exit()


# time.sleep(0)

AI = pyttsx3.init()

while (1):
    voice_data = Audio_record(
        "Ready to take your commands")
    print("Done")
    print("Q:", voice_data)
    respond(voice_data)
