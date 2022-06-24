from distutils import text_file
import yaml, os, shutil
from ase import Atoms

def copy_POTCAR(element):
    
    '''Read the element and copy the POTCAR '''
    
    #initial_dir = os.getcwd()
    potcar_folder = "potpaw_PBE.54/"
    pot_dir = element + "_potcar"

    if os.path.exists(element + "_potcar/POTCAR"):
        os.remove(element + "_potcar/POTCAR")

    shutil.copy2(potcar_folder + element + "_GW/POTCAR" , pot_dir)
    

def search_string_in_file(file_name, string_to_search):

    """Search for the given string in file and return lines containing that string,
    along with line numbers"""
    
    line_number = 0
    list_of_results = []
    
    ''' Open the file in read only mode'''

    with open(file_name, 'r') as read_obj:

        '''Read all lines in the file one by one'''

        for line in read_obj:
            
            '''For each line, check if line contains the string'''
            
            line_number += 1
            if string_to_search in line:
                
                '''If yes, then add the line number & line as a tuple in the list'''
                
                list_of_results.append((line_number, line.rstrip()))
   
    '''Return list of tuples containing line numbers and lines where string is found'''
   
    return list_of_results

def create_IN(cut, element):

    atoms = Atoms(element)
    NumA = str(atoms.get_atomic_numbers()[0])
    pot_dir = element + "_potcar/"
    copy_POTCAR(element)

    with open(pot_dir + "VTOTAL1.ae", "r+") as f_file:
        txt_file = f_file.readlines()

    lp = search_string_in_file(pot_dir + "POTCAR", "local part")[0][0]
    n_down = search_string_in_file(pot_dir + "/" + "VTOTAL1.ae", "Down potential follows")[0][0]
    m_up = search_string_in_file(pot_dir + "/" + "VTOTAL1.ae", "Up potential follows")[0][0]

    k = len(list(txt_file[n_down-2].strip().split(" ")))
    aaaa = str(int((m_up - n_down-2)*4 - k))
    bbbb = str(int(n_down + 1))

    with open("IN","w") as in_f:
        in_f.write("VTOTAL1.ae           = file of radii \n")
        in_f.write("    1 " + aaaa + " = JUMPS,nupon\n")
        in_f.write("VTOTAL1.ae           = atomic potential file\n")
        in_f.write("  " + bbbb +" = JUMPS\n")
        in_f.write("VTOTAL1.ae-05p       = -1/2 ionic potential file\n")
        in_f.write("  " + bbbb + " = JUMPS\n")
        in_f.write("  " + NumA + "  " + str(cut) + "  1.000000 = n   CUT  amplitude\n")
        in_f.write("  " + bbbb + " = JUMPS\n")
        in_f.write("POTCAR               = POTCAR file\n")
        in_f.write("   " + str(lp) + " 1000 = JUMPS, number of k's\n")
        in_f.write("POTCARnew            =  POTCARnew file\n")
        in_f.write("(f20.15)                                 = format of Kmax in file POTCAR\n")
        in_f.write("(5e16.8)                                 = format of V(k) in file POTCAR\n")


    if os.path.exists(pot_dir + '/' + "IN"): 
        os.remove(pot_dir + '/' + "IN")
        shutil.move("IN", pot_dir)
    else:
        shutil.move("IN", pot_dir)

def create_files(element, extension):

    tempdir = element + "_dir" + extension

    if os.path.isdir(tempdir):
        shutil.rmtree(tempdir)
        os.mkdir(tempdir)
        os.chdir(tempdir)
    else:    
        os.mkdir(tempdir)
        os.chdir(tempdir)


    with open("../elements/" + element + ".inp", "r+") as f_file:
        txt_file = f_file.readlines()

    with open("INP","w") as inp_f:
        inp_f.write('# \n')
        inp_f.write('# ' + element + "\n")
        inp_f.write("   ae      O\n")
        if len(element) > 1:
            inp_f.write(" n="+ element +" c=pb\n")
        else:
            inp_f.write(" n="+ element +"  c=pb\n")
        inp_f.write("    " + txt_file[3])
        inp_f.write(txt_file[4])
        inp_f.write(txt_file[5])
        if extension == ".ae-05":
            xx = [name for name in list(txt_file[6].split(" "))]
            temp_x = float(xx[13])-0.500
            temp_x = str(temp_x)+"00"
            xx[13] = temp_x
            yy = ' '.join([str(elem) for elem in xx])
            inp_f.write(yy)
        else:
            inp_f.write(txt_file[6])

        inp_f.write(txt_file[7])
        inp_f.write(txt_file[8])
        inp_f.write("100 maxit\n")
        inp_f.write("#" + txt_file[9]+"\n")
        inp_f.write(txt_file[11])

    os.system("./../atm_cGuima/atm_cGuima3")
    files_list = ["INP", "VTOTAL0", "VTOTAL1","VTOTAL2", "VTOTAL3", "OUT"]
    
    for file in files_list:
        os.rename(file,file + extension) 

    ''' Change to the initial dir '''
    try:    
        os.chdir(initial_dir)
        print("Directory changed")
        print(os.getcwd())
    except OSError:
        print("Can't change the Current Working Directory")    
        print(os.getcwd())

    str_list = ''.join(str(e)+", " for e in files_list)
    
    '''Moving the files to the POTCAR folder'''

    pot_dir = element + "_potcar"

    if os.path.isdir(pot_dir):
        pass
        #print(extension + " " + pot_dir + " Already created!")
    else:    
        os.mkdir(pot_dir)
        #copy_POTCAR(element)


    for f_file in files_list:
        if os.path.exists(pot_dir + '/' + f_file + extension): 
            os.remove(pot_dir + '/' + f_file + extension)
            shutil.move(tempdir + '/' + f_file + extension, pot_dir)
        else:
            shutil.move(tempdir + '/' + f_file + extension, pot_dir)

    #[shutil.move(tempdir + '/' + f_file + extension, pot_dir) for f_file in files_list]
    shutil.rmtree(tempdir)

    return print("Extension: " + extension + " Files " + str_list  + "were successfully created and renamed.")

if __name__ == '__main__':
    
    with open('rendered_wano.yml') as file:
        wano_file = yaml.full_load(file)

    element = wano_file["Element"]
    initial_dir = os.getcwd()
    extensions = [".ae", ".ae-05"]
    cut = 2.0

    [create_files(element, ext) for ext in extensions] 

    create_IN(2.0, element)
    os.chdir(element + "_potcar")
    os.system("./../program_m05/add2POTCARF "+ str(cut) +" 1.0")
    os.chdir(initial_dir)