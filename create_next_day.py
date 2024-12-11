import os

def create_next_day():
    # get all folders in current directory
    folders = [f for f in os.listdir('.') if os.path.isdir(f)]
    # order folders by name
    folder_numbers = [int(f[3:]) for f in folders if f[:3] == 'day']
    last_folder_number = max(folder_numbers)
    # create the next folder
    next_folder = 'day' + str(last_folder_number + 1)
    os.mkdir(next_folder)
    # copy main.cpp as dayX.cpp
    os.system('cp main.cpp ' + next_folder + '/' + next_folder + '.cpp')
    # create 2 empty input.txt and input.test
    os.system('touch ' + next_folder + '/input.txt')
    os.system('touch ' + next_folder + '/input.test')


if __name__ == '__main__':
    create_next_day()
