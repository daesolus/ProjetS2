//
//  InterfaceController.h
//  remoteColor WatchKit Extension
//
//  Created by Felix Lapalme on 2015-03-28.
//  Copyright (c) 2015 Felix Lapalme. All rights reserved.
//

#import <WatchKit/WatchKit.h>
#import <Foundation/Foundation.h>

@interface InterfaceController : WKInterfaceController
@property (weak, nonatomic) IBOutlet WKInterfaceGroup *mainGroup;
@property (nonatomic, strong) MMWormhole *wormhole;

@end
