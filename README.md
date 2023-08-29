# OpenFileClassifier

OpenFileClassifier is Windows application to easily classify files in a folder according to their extension. The user chooses the source and output folder, and the software will, by default, classify the files in folders according to their extension, so all `jpeg` files will be stored in a folder called `jpeg` and so on. Pretty useful to organize your _Downloads_ folder, which is exactly why I built this software in the first place.

![image](https://github.com/JoseTomasTocino/FileClassifier/assets/648833/53571ec4-1364-4513-96ec-61d2c3ac7804)

The currently available options are:
* Copy files to folders instead of moving them.
* Create folders for file extensions (**default behavior**).
* Search recursively.
* Process only media files (images and video).

OpenFileClassifier is also able to further classify media files by checking their EXIF data (only for JPEG files). The _"Classify by"_ drop down allows to select the folder structure for these files, providing classification by year, year and month, and full date.
