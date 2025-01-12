# AMoDSim

An Efficient and Modular Simulation Framework for Autonomous Mobility on Demand.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and simulation purposes. 

### Prerequisites

AMoDSim requires the simulation framework OMNeT++ v4.6.

### Installing

(1) Download OMNeT++ 4.6 using the link below
    <https://omnetpp.org/component/jdownloads/summary/32-release-older-versions/2290-omnet-4-6-source-ide-tgz>

(2) Install OMNeT++ 4.6: follow the step by step installation guide under omnetpp-4.6/doc/InstallGuide.pdf.

(3) Clone the AMoDSim simulator into the Omnet++ workspace

*  Open a shell and go to the Omnet++ workspace (if you do not know what your workspace is, check Window/Preferences and type "Workspaces" into the search box):

    `cd /path/to/workplace/folder`

*  Clone the project to your Omnet++ workplace directory

    `git clone -b <branch> https://gitlab.com/andreadimaria/amodsim.git ./AMoD_Simulator`

    Branch is the name of the branch you want to work with. It can be `master` or `develop`
 
(4) Create an empty project into the Omnet++ workspace

   ```
    File → new → Omnet++ Project 
            - ProjectName: AMoD_Simulator
            - use default location
            - Support C++ Development
   ```
Then, "Create an empty project".
At this point, your project should already contain all the files needed.

**IMPORTANT:**
From within Omnet, delete the package.ned automatically created by Omnet into AMoD_Simulator/. If you don't find this file from within Omnet, delete it from outside, e.g., from command line.
 
 
(5) Build the project

    `Right click on project → Build Project`
    
(6) Now you can run the simulation example

    `Right click on omnetpp.ini located in simulations folder, then click on run as → Omnet++ Simulation`

## Authors

* **Andrea Di Maria** 

    Aucta Cognitio R&amp;D Labs, Catania 95123, Italy
    
    <andrea.dimaria90@gmail.com>, <adimaria@auctacognitio.net>

## Contributors

* **Andrea Araldo**

    Massachusetts Institute of Technology, Cambridge MA 02139, USA
    
    <{last}@mit.edu>

* **Giovanni Morana** 

    Aucta Cognitio R&amp;D Labs, Catania 95123, Italy
    
    <gmorana@auctacognitio.net>
    
* **Antonella Di Stefano**

    Università di Catania, Catania 95125, Italy
    
    <ad@dieei.unict.it>

## License

This project is licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) License - see the [LICENSE.md](LICENSE.md) file for details