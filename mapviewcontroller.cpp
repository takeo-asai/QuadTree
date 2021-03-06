#include "mapviewcontroller.h"

#include "mapview.h"
#include "mainwindow.h"


#include <QPixmap>
#include <vector>
#include "imageview.h"



#include "region.hpp"
#include "location.hpp"
#include "ladybug.hpp"

MapViewController::MapViewController() {

    // TODO: あたりは右上
    Region region = Region(140, 40, 5, 5);   //  Regionは全体

    //  create ladybugs
    this->tree = new QuadTree<Location>(region);
    this->ladybugs = new std::vector<LadyBug *>();
    for(;;) {
        LadyBug *ladybug = new LadyBug("", "/Users/asai/Desktop/gps.txt");
        std::vector<Location *> *locs = ladybug->getLocations();
        for ( unsigned int i = 0; i < locs->size(); i++ ) {
            if ( !locs->at(i)->isError ) {
                this->tree->add(locs->at(i));
            }
        }
        this->ladybugs->push_back(ladybug);
        break;  //  TODO: add many ladybugs
    }

    //  create MainWindow
    this->mainWindow = new MainWindow();


    //  create MapView
    QPixmap *image = new QPixmap();
    image->load("/Users/asai/Desktop/image.jpg");
    this->mapView = new MapView(this->mainWindow);
    this->mapView->setGeometry(0,0,this->mainWindow->width(), this->mainWindow->height());
    this->mapView->setImage(image);
    this->mapView->setLocations(this->ladybugs->at(0)->getLocations()); //  TODO: 見せる範囲を指定する
    this->mapView->setViewController(this);
    this->mapView->setRegion(&region);


    this->mainWindow->show();
}

MapViewController::~MapViewController() {
    delete this->mainWindow;
    delete this->tree;
}


void MapViewController::showRegion(Region region) {
    QPixmap *image = new QPixmap();
    image->load("/Users/asai/Desktop/image.jpg");


    Location *loc = this->tree->findFirstObjectInRegion(region);
    if ( loc != NULL ) {
        printf("found (%f, %f) in ", loc->x, loc->y);

        ImageView *imageView = new ImageView();
        imageView->setImage(image);
        imageView->show();
    } else {
        printf("(140.85316324, 38.03759849) NOT 404 found in ");

        region.dump();
    }



}
