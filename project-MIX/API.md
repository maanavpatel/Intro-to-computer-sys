# Project MIX: API Documentation


## Overview:
Any Information Microservices or "IM"s that wants to be shown will send a request to MIX in order to be added. As new APIs are added the result of MIX will dynamically change. All IMs have the power to update and delete themselves from MIX at anytime. MIX will store the name and url as a key to retrieve information. 

---

## POST requests:

### `/MIX`

- **Requirement:**\
    Send a request with data about the coordinates that you want information for. Coordinates are recieved through a `form` and should be in the following format: `<latitude>,<longitude>`. 
- **Response:**
    A JSON like:
        {
        "Name": Name of IM,
        "Info":  Data Provided By IM
        }

### `/MIX/IM/add`

Send a request with data about the IM you would like to register or update:

- **Requirement:**\
    Send a request with data about the IM you are registering. IM info are recieved through `JSON` and should be in the following format: 

        {name: <name>, url: <url>}


- **Response:**
    A JSON like:

    `200:` The IM existed in `MIX` and was updated\
    `201:` The IM did not exist in `MIX` and was added\
    `400:` The data sent was not formatted correctly


### `/MIX/IM/delete`

Send a request with data about the IM you would like to delete

- **Requirement:**\
    Send a request with data about the IM you are deleting. IM info are recieved through `JSON` and should be in the following format: 

        {name: <name>, url: <url>}


- **Response:**
    A JSON like:

    `404:` The IM did not exist in `MIX` so it was not deleted\
    `200:` The IM did extist in `MIX` and was deleted


---

## Dependencies

required technologies:\
+ Python
+ pip or similar package installer
    + Python Packages:
        + flask
        + reqests
        + logging

