import xml.etree.ElementTree as ET
import pandas as pd
import sys, os, json

START_OF_TEMPLATE = 47


class sc_parser:
    xmlData = []
    gpio = []
    peripherials = []
    filename = ""

    partnumber = ""
    device = ""
    pinCount = 0
    deviceCode = {}

    def __init__(self, filename):
        self.filename = filename

        self.sc_parser()
        self.sort_info()

        for f in self.peripherials:
            f = self.rza1_getAlternateMode(f)

    def sc_parser(self):

        tree = ET.parse(self.filename)
        root = tree.getroot()

        # Get Board information
        self.device = root.find('Device').attrib['Family']
        self.partNumber = root.find('Device').attrib['PartNumber']
        self.pinCount = int(root.find('Device').attrib['PinCount'])

        # Get pins for all selected peripherials
        pins = root.find('Pin')
        for p in pins:
            self.xmlData.append(p.attrib)

    def sort_info(self):
        for p in self.xmlData:
            if p['Id'] == p['Port']:
                self.gpio.append(p)
            else:
                self.peripherials.append(p)

    def printDeviceInfo(self):
        print(self.device)
        print(self.partNumber)
        print(self.pinCount)

    def printGPIO(self):
        for gpio in self.gpio:
            print(gpio)

    def printFunctions(self):
        for f in self.peripherials:
            print(f)

    def rza1_getAlternateMode(self, func):
        ID = func["Id"]
        pin = func["Port"]
        altr_md = ""
        if (self.device == "RZA1LU"):
            filename = "rza1lu_pinmap.json"
        else:
            print("Error Device cannot be found")

        pwd = os.getcwd() + '\\'
        fp = open(pwd + filename)

        jData = fp.read()
        rza1 = json.loads(jData)

        if pin in rza1:
            funcs = rza1[pin]
            for f in funcs:
                if (f["Func"] == ID):
                    func['Alternate'] = int(f['Alter'])
                    func['Direction'] = f['DIR']

        else:
            print("Error pin: " + pin + " does not exist")

        return func
    def createDevFuncEntryTable ( self ):
        pwd = os.getcwd() + '\\'
        df = pd.read_csv(pwd + 'rza1l_devices.csv')
        devs = df['Device'].drop_duplicates().values
        for d in devs:
            self.deviceCode[d] = []

        for p in self.peripherials:
            if (len(df[df['Function']==p['Id']]) <= 0) :
                print("Error no function with ID: "+p['Id'])
                break;
            dev = df[df['Function']==p['Id']].values[0][1]
            if len(dev) > 0:
                """
                if ( self.deviceCode != {} or dev in self.deviceCode):
                    pass
                else:
                    self.deviceCode[dev] = []
                """
                entry = "\t{"+p['Port']+',\t'+'FUNCTION_MODE'+str(p['Alternate'])+',\tPIN_'+p['Direction']+'},\t// Pin Function : '+p['Id']+'\n'
                self.deviceCode[dev].append(entry)
            else:
                print("Error no function "+p['Id'])
        # print(self.deviceCode)


    def rza1CreateCCode(self):

        pwd = os.getcwd() + '\\'
        fp = open(pwd + 'r_gpio_cfg.h', 'w')
        tp = open(pwd + 'header_template.h')

        # Added Renesas Header
        # Add macro dupicate definition checker
        for line in range(START_OF_TEMPLATE):
            l = tp.readline()
            fp.writelines(l)

        # Add GPIO array

        # Add array for each periherial
        self.createDevFuncEntryTable()
        # Populate periherial
        for key, value in self.deviceCode.items():
            # Write Device entry
            device = "static const st_port_config_t GPIO_SC_TABLE_"+key.lower()+"[] =\n{\n"
            fp.writelines(device)

            for entry in value :
                fp.writelines(entry)
            fp.writelines('};\n')

            wrapper = "static const st_port_init_config_t GPIO_SC_INIT_"+key.lower()+" = { "
            fp.writelines(wrapper)
            wrapper = "0,\t"+str(len(value))+',\t'+'&GPIO_SC_TABLE_'+key.lower()+'[0]'
            fp.writelines(wrapper)
            fp.writelines('\t};\n')

        # Append end of files
        for line in range(3):
            l = tp.readline()
            fp.writelines(l)

        fp.close()
        tp.close()


if __name__ == "__main__":
    sc = sc_parser(sys.argv[1])
    #sc.printFunctions()
    sc.rza1CreateCCode()