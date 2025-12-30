# Canvas File Explorer - Raw Interview Brainstorm

Date: 2025-12-29

## Q&A Session

**Q: What's the feature you want to build? Give me the elevator pitch.**
A: it's a new ui feature and sort of a new data feature. but basically i want to make a file explorer

**Q: When you say 'file explorer' — are you exploring actual filesystem files, or entities in your graph database that represent file-like things?**
A: probably both in some sense, but for now i would say real files and the combine it with arx_caeli. the idea to have half screen or full screen desktop areas. you would probably be able to browse the whole host file system, but you would also mount folders / alias files from host operating system so it would almost end up being like a cross between arx_caeli/miro, obsidian and the finder.

**Q: How does this relate to NavigatorEntitatum which already explores the entity graph?**
A: Complement

**Q: For the 'desktop areas' concept — is this spatial positioning like Miro (infinite canvas with placed items) or more like macOS Stacks/desktop icons (grid-based)?**
A: it would be finite canvas. either fullscreen or half screen. probably half screen but you might have two panes open at the same time sometimes. the idea is that you can drag the icon for files wherever you want but no scrolling, no overlapping windows just infinite "desktop" views that you can also probably place notes of different types on alongside file icons

**Q: When you 'mount' or 'alias' a folder from the host OS — does this create entities in your graph, or is it a live view that queries the filesystem?**
A: live view but you would probably be able to annotate files with additional metadata and i guess it would keep track of that file from that mount as an entity because you might be able to mount/alias the same file multiple times. so that's another goal to allow files to exist in multiple locations in multiple contexts

**Q: For 'notes of different types' on the desktop — what note types are you imagining? Text snippets, images, links, todos, or something else?**
A: Mini-widgets, Typed notes, Rich content, Text only, right now there are only text notes and "folders" which are links to other canvases that you can drag and drop things onto. but in the future i could imagine supporting image notes and maybe entity view notes. possibly things like calendar and clock as well

**Q: When a file exists in multiple desktop locations (aliased) — are changes to annotations synced across all instances, or is each alias independent?**
A: probably closer to layered. its kind of like domain specific vocabulary for files. the same file might kind of mean different things in different contexts. but you would certainly be able to see all the versions or annotations of a file across the different locations in one place

**Q: For navigating between desktop canvases — you mentioned folders link to other canvases. Is this hierarchical (tree of desktops) or flat (any canvas can link to any other)?**
A: i think flat graph, any canvas links to any canvas, kind of like hyper links more than a folder tree per se (although "addressses" should provide the ability to have multiple levels of namespacing)

**Q: How do you envision the relationship between these desktop canvases and Schirmata screens? One canvas per screen, or canvases are independent of screens?**
A: Canvas in widget

**Q: For the 'addresses with namespacing' — what does a canvas address look like? URL-style paths, or something else?**
A: URL-style paths

**Q: When you're viewing a canvas and want to open a file — what happens? Opens in a new pane, replaces the canvas, opens in system app?**
A: probably an internal launcher view (maybe in the facing widget panel) that gives you options for different actions you can take iwth the file including an internal viewer or macos system app launch

**Q: For the filesystem browsing part — when you navigate into a mounted folder, does it show contents as icons on the canvas, or switch to a list/tree view?**
A: Canvas icons

**Q: How do you handle large folders (thousands of files)? The finite canvas can't fit them all.**
A: pagination, maybe kind of like a mobile phone screen. with some drop target to move files to previous/next page, etc? definitely no scrolling though and since they are icons that are not snapped to grid i think some kinds of searching don't make sense because you kind of want object permanence and things to stay in the same position until explicitly moved by the user. part of the idea is that it might be useful for task based projects where you would have all the files for the project in one place, etc

**Q: For drag-and-drop between pages — can you drag an item to the edge to trigger page change, or do you need explicit 'move to page X' action?**
A: proabably explicit drop zones. but we are kind of trying to do acme commands there might be the ability to type some command then click it to move files as well

**Q: For the 'task-based project' use case — do canvases have metadata like status, dates, tags, or is that just on individual items?**
A: Both

**Q: For the Acme-style commands — where does the command text live? A text area on the canvas, a command bar, or inline with items?**
A: my vision is that anywhere you can type text you can type acme commands so you probably could make a text note and type a command in it if you wanted to.

**Q: How does this relate to your existing RegistrumCommandi command registry? Same commands, or canvas-specific commands?**
A: i want to have "commands" ie any action the user can take to cause a mutation, etc be a globally registered almost "os level" thing so at any time a user can see all their available commands through different kinds of ui. so depending on visible widgets certain commands might not be available because their requirements / parent is not set but commands should be a global layer

**Q: For icon rendering — do files show thumbnails (image preview, PDF first page), generic type icons, or custom user-assigned icons?**
A: Type icons

**Q: What's the interaction model for selecting and manipulating items? Click-to-select, drag-to-move, right-click menu?**
A: definitely click to select, drag to move. i'm not sure what else will be available but possibly something like right click menu as well. wouldn't worry to much about vim navigation because this is a explicitly a mouse first kind of mode. another thing before i forget is that i think i want any file to be able to be a "folder" so in theory any file can have a canvas associated with it and other files inside of it alongside it's literal data

**Q: For 'any file can be a folder' — is this stored as entity relationships (file A contains file B), or does it modify the actual filesystem?**
A: Entity relationships

**Q: When a file 'contains' other files — are those files still accessible at their original filesystem location, or do they move?**
A: that's an interesting question. part of the idea is to expand what the "file system" can do and not be limited by the constraints of current file systems. so i don't really imagine the organization stuff changing the host system folder structure/locations, this would be parallel structures a file can be a part of. at the same time i would like this to eventually be capable of being a "daily driver" finder replacement, so maybe you would have a different widget or mode that is based on more literal file system manipulation and viewing

**Q: For the 'daily driver Finder replacement' goal — what filesystem operations need to work? Copy, move, delete, rename, create folder?**
A: probaby staged development over time, but i imagine you would end up with crud operations, ability to launch the file in a native macos app, ability to launch and navigate native macos apps(?) and ability to launch/interact with through rhubarb widgets, etc internally. part of the vision is that user data in rhubarb is able to exist as a single file, kind of like a sqlite database, so it's very easy to copy and backup your data. but that data can also be mounted on the filesystem in various schemes as a kind of projection that is kept up to date and that there would be a lot of tools to export/import subsets of your data as desired

**Q: How do you handle file changes made outside your app (in Finder, terminal, other apps)?**
A: probably fsevents watch or polling. but not 100% sure. one ui paradigm i want to explore is having an "inbox" for the user that can have tasks assigned to them by the system and information for them to review. so it's possibly that filesystem changes to mounted folders could trigger an aggregated series of notifications/review tasks where the user could manually decide what action to take for each. in some ways i prefer this model because it lets the user know what changes are happening vs having it be a big unknown how and where things are changing especially in large data graphs

**Q: For the 'inbox' notification model — is this a separate widget/canvas, or a notification area within the file explorer?**
A: Separate widget

**Q: For the single-file database export — does this include file contents, or just metadata/organization?**
A: probably at some point as you are mounting host files they will be mirrored into the database so that data is available to you if you move to a different computer. probably at a certain point to the user there would be no distinction between file system files and "rhubarb" virtual files that only exist in the database. both could be interacted with in most of the same ways as long as you are staying inside of rhubarb, being a file system file would just be giving you some extra operations i guess

**Q: For file mirroring into the database — is this automatic (all mounted files get copied in), or selective (user marks files to import)?**
A: probably would be automatic and at that point you would start to keep an event sourced history of the content of files based on polling/hashes/merkle trees or something like that. but there would be some amount of opt in or opt out. obviously there would probably be some things you would not want to be forced to copy into your database. maybe you would mark them as "computer specific file" or something

**Q: What's the first milestone you'd want working? Full canvas system, or something narrower?**
A: Mount + icons

**Q: Does arx_caeli already have canvas-like features you want to build on, or is this a new widget from scratch?**
A: New widget

**Q: Any final thoughts or requirements I haven't captured?**
A: all covered! and if you can keep the raw brainstorm conversation with our questions and answers as well as creating the spec that would be great
