#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#include <unistd.h>

int main()
{
    // // Create the File Open Dialog class.
    // NSOpenPanel* openDlg = [NSOpenPanel openPanel];
    // NSLog(@"SADSAD");
    // // Enable the selection of files in the dialog.
    // [openDlg setCanChooseFiles:YES];
    // NSLog(@"SADSAD");

    // // // Multiple files not allowed
    // [openDlg setAllowsMultipleSelection:NO];
    // NSLog(@"SADSAD");
    
    // // // Can't select a directory
    // [openDlg setCanChooseDirectories:YES];
    // NSLog(@"SADSAD");

    // // Display the dialog. If the OK button was pressed,
    // // process the files.
    // // [openDlg setLevel:1];
    // // [openDlg beginWithCompletionHandler:^(NSInteger returnCode) {
    // //     NSLog(@"completionHandler called with %d", returnCode);
    // //     }];

    // // [openDlg beginWithCompletionHandler:^(NSInteger result) {

    // //     //if the result is NSOKButton
    // //     //the user selected a file

    // //         if (result==NSModalResponseOK) {

    // //             //Do something
    // //                             NSLog(@"OK");

    // //         }
    // //         else if (result== NSModalResponseCancel)
    // //         {
    // //             //do something
    // //             NSLog(@"BI");
    // //         }
    // // }]; 
    //     int code = [openDlg runModal];

    // // NSLog(@"%d", NSModalResponseCancel);
    // if ( code == NSModalResponseOK )
    // {
    //         NSLog(@"IN");

    //     // Get an array containing the full filenames of all
    //     // files and directories selected.
    //     NSArray* urls = [openDlg URLs];

    //     // Loop through all the files and process them.
    //     for(int i = 0; i < [urls count]; i++ )
    //     {
    //         NSString* url = [urls objectAtIndex:i];
    //         NSLog(@"Url: %@", url);
    //     }
    // }
    //      NSLog(@"OUT");

    dispatch_async(dispatch_get_main_queue(), ^{
                NSLog(@"DATA");

    NSOpenPanel* openDlg = [NSOpenPanel openPanel];
    [openDlg setCanChooseFiles:YES];
    [openDlg setCanChooseDirectories:NO];
    [openDlg setAllowsMultipleSelection:NO];
    // NSArray * fileTypes = [NSArray arrayWithObject:@"json"];
    // [openDlg setAllowedFileTypes:fileTypes];
           [openDlg setLevel:1];

      if ([openDlg runModal] == NSModalResponseOK)
      {
        //   const char * filePath = [[openDlg URL] path].UTF8String;
        //   strcpy(pathToOpen, filePath);
        //   OperationData opData("open gears project");
        //   opData.callback = callback;
        //   opData.args = (void*)(pathToOpen);
        //   COperationsQueue::queueOperation(opData);
        NSLog(@"DATA");
      }
    });

    return 0;
}