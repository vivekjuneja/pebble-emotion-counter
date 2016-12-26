# pebble-emotion-counter
An attempt to build a simple Pebble Application for capturing emotion states for later analysis


![Screeshot of the App]
(https://github.com/vivekjuneja/pebble-emotion-counter/raw/master/resources/images/emoter-app.jpg)


**Brief Process**

I used Pebble 4 SDK and PebbleKit JS to build a simple application for Pebble 2 watch. The goal is to build a simple 
application that can capture the three different emotions - Happiness, Sadness and Anger. We use the data to understand 
how emotions vary across a day, and that then can be used to build powerful analytics to understand the trigger points 
for emotions for a person.

**How did I build it**

I used CloudPebble, the amazing Cloud IDE for Pebble app developers. I build the WindowLayout and then added the various events 
using C language. I also use `AppMessage` to communicate to Webview layer, which is powered by PebbleKit JS. This Javascript
allows to connect to remote backend for persisting the data collected over time. We use mLAB, a simple MongoDB hosted service 
and created a rudimentary collection to capture these emotions. mLAB exposes RESTFul interface to its Database and Collection
services. 

**Roadmap**

1. Allow offline mode for the watch, when its not connected to the Phone. 

2. Allow recording of voice message for each emotion so that user can capture the reason for the emotion.

3. A Simple Web Front to consume and present the data collected by the App 

4. Vibrating nudge / Phone notification to remind the user to record the emotion

5. Release on Pebble App Store

