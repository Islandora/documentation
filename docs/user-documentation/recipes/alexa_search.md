# Accessing Islandora with Amazon Alexa

The following recipe details how to connect Islandora with Alexa, using custom Alexa skills and the Drupal Alexa module. The potential applications are broad:
- Send Citations, metadata, whatever we want to the user if they have set up their email
- Creating a collaborative exhibit
- Play audio and video objects and read transcripts
- Respond to user search queries (like how many objects match the subject in the repositories)
- Answer specific questions about the object (“_Invocation Name_, when was this recorded?”)
- Interact with other applications or modules (got a print ordering system? Want to add event calendar items to your exhibit?)
- Be accessed via web page, Alexa device, or phone app 

!!! note "Note"
    This recipe has not been extensively tested.

## Ingredients

- Drupal 8 
- Islandora
- Drupal Alexa Module
- Islandora Oral Histories Module
- Custom Alexa Skill
- Custom Search 

## Instructions

1. Create an Amazon Skill 
     1. Log in https://developer.amazon.com go to Alexa > Skills Kit > add new skill
     1. Pick an _Invocation Name_ 
     1. Configure SSL etc (Tutorial: https://www.drupal.org/docs/8/modules/alexa/tutorial) 
     1. Save config, write down the Application ID on the Skill Information Tab!
1. Integrate with Drupal
     1. Install the Alexa module using Composer:  `composer require "drupal/alexa"`
     1. Enable modules (Alexa, Alexa_Demo) 
     1. Go to Config > Alexa Configuration and put in the Application ID. 
1. Test with one question and answer (“Clawbster, say Hello world?” “Hello World!”) Now you can add as many questions and answers as you can configure:
![Screenshot of google form with questions and answers for chatbot](../../assets/recipe_alexa.png)
