# Running the example
Launch the image server
```sh
# from within the build directory
bin/image_server
```
Now that the image server is running, you need to send it an image to process. There are many ways to do this. I will explain two of them here: either via the command line using `curl`, or via a GUI using [Postman](https://www.postman.com/downloads/).

## Command line: `curl`
Make sure that the image server is running. In another console window
```sh
curl localhost:8080/transform/oilpainting --data-binary "@/path/to/input-image.jpg" --output ./output-image.jpg
```
After this command has completed you should have a new file in your dirctory called `output-image.jpg`.


Check out `handle_requests.hpp` for more urls to image processing functions other than `transform/oilpainting`.

## GUI: Postman
From the *Overview* tab click *Create A Request*. Select `POST` from the dropdown menu. Enter the following url in the request URL address bar.
```
localhost:8080/transform/oilpainting
```
> Note: If the server is running in WSL2 you need to replace `localhost` with the IP address of your WSL instance. Type `ifconfig` in your WSL console and use the IP address listed after `inet` in the resulting output. For example if you see
>```sh
>$ ifconfig
>eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1350
>        inet 172.19.196.213  netmask 255.255.240.0  broadcast 172.19.207.255
> ...
>```
> then replace `localhost` with `172.19.196.213` in the Postman request URL address bar
>```sh
>172.19.196.213:8080/transform/oilpainting
>```



Under the address bar select the *Body* tab and the *binary* radio button. Press the *Select File* button and select an image to send as request.

Press *Send*.

After the response has arrived (less than a second), click the *Save the Response* dropdown and select *Save to File*. Be sure to save the file with the .jpg extension, for example `response.jpg`.

## What success looks like
If you open the file in an image view you should see something very much like this:

![Lenna as an oil painting](response.jpg)
