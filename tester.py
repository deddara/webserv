import requests
#  from requests.auth import HTTPBasicAuth
import sys


def print_response(resp):
    print("----------------------- Response ----------------------------------")
    print("Status: {}".format(resp.status_code))
    print('')
    for key, val in resp.headers.items():
        print(key + ': ' + val)
    print("\n" + resp.text)


def get_request(uri):
    print("----------------------- Request -----------------------------------")
    host = "http://" + sys.argv[1] + ":" + sys.argv[2]
    print("Request: GET on " + host + uri)
    input("Press Enter to continue...")
    print_response(requests.get(host + uri))


def head_request(uri):
    print("----------------------- Request -----------------------------------")
    host = "http://" + sys.argv[1] + ":" + sys.argv[2]
    print("Request: HEAD on " + host + uri)
    input("Press Enter to continue...")
    print_response(requests.head(host + uri))


def put_request(uri, body):
    print("----------------------- Request -----------------------------------")
    host = "http://" + sys.argv[1] + ":" + sys.argv[2]
    print("Request: PUT on " + host + uri)
    input("Press Enter to continue...")
    print_response(requests.put(host + uri, data=body))


get_request('/')
head_request('/')
put_request('/put_test/testfile.txt', 'foo bar baz')
