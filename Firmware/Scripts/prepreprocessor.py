
from os import listdir, getcwd
from os.path import isfile, isdir, join

# Constants
PATH_TO_PROJECT_HOME = "../"
DIRECTORIES_TO_CHECK = ["Application"]
SOURCE_FILE_EXTENSIONS = ["c","cpp"]
HEADER_FILE_EXTENSIONS = ["h","hpp"]
FILE_EXTENSIONS_TO_CHECK = SOURCE_FILE_EXTENSIONS + HEADER_FILE_EXTENSIONS


def flatten_2D(xss):
    return [x for xs in xss for x in xs]


def get_files(directory):
    files = [join(directory, f) for f in listdir(directory) if isfile(join(directory, f))]
    for subdirectory in [join(directory, d) for d in listdir(directory) if isdir(join(directory, d))]:
        files += get_files(subdirectory)
    return files


def filter_files(files):
    
    # Remove duplicates
    filtered_files = list(dict.fromkeys(files))

    # Remove invalid file types
    for file in filtered_files.copy():
        if file.split(".")[-1] not in FILE_EXTENSIONS_TO_CHECK:
            filtered_files.remove(file)
            
    return filtered_files


def insert_user_code(file_name, section, text):

    previous_contents = ""
    new_contents = ""

    start = "USER CODE BEGIN " + section
    end = "USER CODE END " + section
    
    with open(PATH_TO_PROJECT_HOME + file_name, "r") as file:
        previous_contents = file.read()

    new_contents = previous_contents.split(start,1)[0] + start + " */\n" + text + "\n/* " + end + previous_contents.split(start,1)[1].split(end,1)[1]

    with open(PATH_TO_PROJECT_HOME + file_name, "w") as file:
        file.write(new_contents)


def remove_start_and_end_comments(text):

    # Remove first end comment
    text = text.split("*/",1)[1]

    # Remove last start comment
    text = "/*".join(text.split("/*")[:-1])
    
    return text


def replace_file(file_name, text):

    #  Check if file is a header so include guards can be inserted
    if file_name.split(".")[-1] in HEADER_FILE_EXTENSIONS:
        guard_name = file_name.split("/")[-1].split("\\")[-1].replace(".", "_").replace(" ", "_").upper() + "_"
        text = "#ifndef " + guard_name + "\n#define " + guard_name + "\n\n" + text + "\n\n#endif /* " + guard_name + " */\n"

    with open(PATH_TO_PROJECT_HOME + file_name, "w") as file:
        file.write(text)


def main():
    files_to_check = filter_files(flatten_2D([get_files(join(PATH_TO_PROJECT_HOME, directory)) for directory in DIRECTORIES_TO_CHECK]))

    
    for primary_file in files_to_check:        
        print(primary_file)

        action = ""
        secondary_file = ""
        section = ""
        text_to_copy = ""
        execute_action = False
        

        with open(primary_file, "r") as file:
            primary_contents = file.read()

        for line in primary_contents.split("\n"):

            if "@ACTION:" in line:
                action = line.split(":", 1)[1].strip()
                
            elif "@FILE:" in line:
                secondary_file = line.split(":", 1)[1].strip()

                if secondary_file[0] == '"':
                    secondary_file = secondary_file[1:]
                if secondary_file[-1] == '"':
                    secondary_file = secondary_file[:-1]
                
            elif "@SECTION:" in line:
                section = line.split(":", 1)[1].strip()

                if section[0] == '"':
                    section = section[1:]
                if section[-1] == '"':
                    section = section[:-1]

            elif "@INSERT:" in line:
                text_to_copy = line.split(":", 1)[1].strip()

                if text_to_copy[0] == '"':
                    text_to_copy = text_to_copy[1:]
                    if text_to_copy[-1] == '"':
                        text_to_copy = text_to_copy[:-1]

                execute_action = True
               
            elif "@START" in line:
                text_to_copy = line
                execute_action = False
                
            elif "@END" in line:
                execute_action = True
            




            if action == "USER_CODE_INSERT" and execute_action == True:

                # Insert the code snippet
                insert_user_code(secondary_file, section, text_to_copy)

                # Reset variables
                section = ""
                text_to_copy = ""
                execute_action = False
                
            elif action == "REPLACE_FILE":

                # Add data to the line to be copied
                text_to_copy += "\n" + line

                
                if execute_action == True:
                    text_to_copy = remove_start_and_end_comments(text_to_copy)
                    replace_file(secondary_file, text_to_copy)

                    # Reset variables
                    text_to_copy = ""
                    execute_action = False
            
            




















main()
































